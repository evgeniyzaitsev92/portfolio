import '../App.css';
import { formatDate, getTemp } from '../utils/helpers.js'

function Weather7Days(props) {
    return(
        <div className="daily frame d-flex flex-dir-col">
            <h2>7 days</h2>
            <div className="daily-list d-flex">
            {props.dailyWeather.map((day) => (
                <div className="daily-item d-flex flex-dir-col ai-cntr jc-sb">
                <h4>{formatDate(day["day"], 'daily')}</h4>
                <img src={require(`../views/img/weather_icons/${day.icon}.png`)} alt="" />
                <h4>{getTemp(day["all_day"]["temperature"])} °C</h4>                    
                </div>
            ))}
            </div>
        </div>
    );
}

export { Weather7Days }