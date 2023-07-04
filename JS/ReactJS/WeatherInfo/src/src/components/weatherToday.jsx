import '../App.css';

function WeatherToday(props) {
    return(
        <div className="frame d-flex flex-dir-col ai-cntr jc-sb">
            <h2>{props.summary}</h2>
            <div className="weather-type-temp d-flex">
            <img src={props.icon} alt="" />
            <h2 className="temp">{props.temp} °C</h2>
            </div>
            <h3>{props.date}</h3>
        </div>
    );
}

export { WeatherToday }