const ORG_ID = '';
const REFRESH_TOKEN = '';
const CLIENT_ID = '';
const CLIENT_SECRET = '';
const SCOPE = 'Desk.tickets.READ,Desk.contacts.READ,Desk.tasks.READ,Desk.basic.READ,Desk.settings.READ,Desk.search.READ,Desk.events.READ,Desk.articles.READ';
const URL_ACCESS_TOKEN = `https://accounts.zoho.com/oauth/v2/token?refresh_token=${REFRESH_TOKEN}&client_id=${CLIENT_ID}&client_secret=${CLIENT_SECRET}&scope=${SCOPE}&redirect_uri=https://www.zylker.com/oauthgrant&grant_type=refresh_token`;
const URL_TICKETS = 'https://desk.zoho.com/api/v1/tickets';
const URL_TICKETS_SEARCH = `${URL_TICKETS}/search?`;
let ACCESS_TOKEN = '';

function formatToMoscowTime(date){
    return new Date(date.setHours(date.getHours()+3));
}

function getEarlierDate(subtractDays){
    const today = formatToMoscowTime(new Date());
    const earlierDate = today.setDate(today.getDate() - subtractDays);
    return new Date(earlierDate);
};

function getDate(date, type){
    let dt;
    if(date === 'today'){
        dt = formatToMoscowTime(new Date());
    } else if(date === 'weekAgo'){
        dt = getEarlierDate(7);
    } else if(date === 'twoWeeksAgo'){
        dt = getEarlierDate(14);
    } else if(date === 'monthAgo'){
        dt = getEarlierDate(30);
    }

    if(type === 'iso'){
        dt = dt.toISOString();
    } else if(type === 'local'){
        const DT = new Date(dt.toISOString().substr(0, 10));
        const year = DT.toLocaleString("en-EN", { year: "numeric" });
        const month = DT.toLocaleString("en-EN", { month: "long" });
        const day = DT.toLocaleString("en-EN", { day: "2-digit" });
        dt = month + " " + day + ", " + year;
    }

    return dt;
};

async function fetchData(metric, period){
    // Period
    const isoToday = getDate('today', 'iso');
    const today = new Date(isoToday);
    let isoPeriod;

    if(period === 'week'){
        isoPeriod = getDate('weekAgo', 'iso'); 
    } else if(period === 'two_weeks'){
        isoPeriod = getDate('twoWeeksAgo', 'iso');
    } else if(period === 'month'){
        isoPeriod = getDate('monthAgo', 'iso');
    } 

    const dtPeriod = new Date(isoPeriod);

    const access = async() => {
        try {
            const data = await fetch(URL_ACCESS_TOKEN, {
                method: 'POST',
            });
            // console.log('access', data.json());
            return await data.json();
        } catch (error) {
            console.log(error);
        }
    };

    const ticketsTotalNum = async() => {
        return await access().then(async json => {
            ACCESS_TOKEN = json['access_token'];        
            
            try {
                const data = await fetch(`${URL_TICKETS_SEARCH}from=1&limit=100&modifiedTimeRange=${isoPeriod},${isoToday}`, {
                    method: 'GET',
                    headers: {Authorization: `Zoho-oauthtoken ${ACCESS_TOKEN}`,
                            orgId: ORG_ID}
                });
                return await data.json();
            } catch (error) {
                console.log(error);
            }
        });
    };

    const tickets = async() => {
        return await ticketsTotalNum().then(async json => {
            const pages = Math.ceil(json['count']/100); // number of pages
            let promises = [];
            let i = 0;

            try {
                while(i != pages){
                    promises.push(await fetch(`${URL_TICKETS_SEARCH}from=${1+(100*i)}&limit=100&modifiedTimeRange=${isoPeriod},${isoToday}&sortBy=modifiedTime`, {
                                        method: 'GET',
                                        headers: {Authorization: `Zoho-oauthtoken ${ACCESS_TOKEN}`,
                                        orgId: ORG_ID}
                    }).then(resp =>  resp.json()));
                    i++;
                }
                const data = Promise.all(promises)
                .then(results => {
                    return results;
                });
                return data;
            } catch (error) {
                console.log(error);
            }
        })
    };
    
    if(metric === 'SLA'){
        const ticketsSLAViolated = async() => {
            let ticketsSLAFormattedInfo = [];

            return await tickets().then(async json => {
                try {
                    for(const page of json){
                        for(const ticket of page['data']){
                            await fetch(`${URL_TICKETS}/${ticket['id']}/History`, {
                                method: 'GET',
                                headers: {Authorization: `Zoho-oauthtoken ${ACCESS_TOKEN}`,
                                        orgId: ORG_ID}
                            })
                            .then(resp => resp.json())
                            .then(json => {
                                let dates = [];
                                for(const event of json['data']){
                                    const eventTime = new Date(event['eventTime']);
                                    if(event['eventName'] === 'OvershotDueTime' && (eventTime >= dtPeriod && eventTime <= today)){
                                        dates.push(new Date(event['eventTime']).toLocaleString());
                                        ticketsSLAFormattedInfo.push({
                                            'ticket': ticket['ticketNumber'], 
                                            'dates': dates,
                                            'url': `https://support.cadexchanger.com/agent/cadexchanger/cad-exchanger/tickets/details/${ticket['id']}`
                                        });
                                    }
                                }                                        
                            })
                        }
                    }
                    return ticketsSLAFormattedInfo;
                } catch (error) {
                    console.log(error);
                }
            });
        };

        return await ticketsSLAViolated();
    }
};

module.exports = {
    fetchData
};