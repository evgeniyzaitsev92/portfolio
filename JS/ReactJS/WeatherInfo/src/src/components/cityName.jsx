import '../App.css';

function CityName(props) {
    return(
        <div className="frame d-flex flex-dir-col ai-cntr jc-sb">
            <h1 className="city">{props.cityName}</h1>
        </div>
    );
}

export { CityName }