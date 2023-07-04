import '../App.css';
import { getTomorrow, formatDate, getTemp } from '../utils/helpers.js'

function Weather24Hours(props) {
    return(
        <div className="hourly frame d-flex flex-dir-col">
            <h2>24 hours</h2>
            <div className="hourly-list d-flex">
            {props.hourlyWeather.map((hour) => (
                <div className="hourly-item d-flex flex-dir-col ai-cntr jc-sb">
                    <h6 className={getTomorrow(hour.date) ? "hourly-item__tomorrow-active" : "hourly-item__tomorrow-deactive"}>Tomorrow</h6>
                    <h5>{formatDate(hour["date"], 'hourly')}</h5>
                    <img src={require(`../views/img/weather_icons/${hour.icon}.png`)} alt="" />
                    <h5>{getTemp(hour["temperature"])} °C</h5>                    
                </div>
            ))}
            </div>
        </div>
    );
}

export { Weather24Hours }