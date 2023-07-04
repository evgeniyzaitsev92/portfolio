import { Autocomplete, CircularProgress, Slide, TextField } from '@mui/material';
import './App.css';
import { apiKey } from './utils/access.js';
import { useEffect, useState } from 'react';
import {CityName} from './components/cityName.jsx'
import { WeatherToday } from './components/weatherToday.jsx'
import { Weather24Hours } from './components/weather24hours.jsx'
import { Weather7Days } from './components/weather7days.jsx'
import { formatDate, getTemp } from './utils/helpers.js'

function App() {

  const [cityName, setCityName] = useState('Male');
  const [inputCity, setInputCity] = useState('');
  const [loadedCity, setLoadedCity] = useState('Malé, Republic of Maldives');
  const [data, setData] = useState({});
  const [options, setOptions] = useState([]);
  const [error, setError] = useState(false);
  const [loading, setLoading] = useState(true);

  useEffect(() => {
    fetch(`https://www.meteosource.com/api/v1/free/point?place_id=${cityName}&sections=all&timezone=UTC&language=en&units=metric&key=${apiKey}`)
    .then((res) => {
      if(res.status === 200){
        setError(false);
        return res.json();
      } else {
        throw new Error('Error!');
      }
    })
    .then((data) => {
      setData(data);
    })
    .catch(() => setError(true))
    .finally(() => setLoading(false))
  }, [cityName, error]);

  useEffect(() => {
    fetch(`https://www.meteosource.com/api/v1/free/find_places_prefix?text=${inputCity}&key=${apiKey}`)
    .then((res) => {
      if(res.status === 200){
        setError(false);
        return res.json();
      } else {
        throw new Error('Error!');
      }
    })
    .then((data) => {
      let options = [];
      for(const item of data){
        if(item['name'].toLowerCase().includes(inputCity.toLowerCase()) || item['place_id'].toLowerCase().includes(inputCity.toLowerCase())){
          options.push({label: `${item['name']}, ${item['country']}`, id: item['place_id']});
        }
      }
      setOptions(options);
    })
    .catch(() => setError(true))
  }, [inputCity]);

  const handleSearch = (e) => {
    setInputCity(e.target.value);
  }

  const hourlyWeather = (data['hourly'] !== undefined) ? data['hourly']['data'] : [];
  const dailyWeather = (data['daily'] !== undefined) ? data['daily']['data'] : [];

  return (
    <div className="bcg jc-center ai-cntr">
      {
        !loading ? (
          <>
            <Slide direction="right" timeout={800} in={!loading}>
              <div className="">
                <Autocomplete
                  className="input"
                  // disablePortal
                  id="search-list"
                  options={options}
                  sx={{ width: 300 }}
                  onChange={(e, newValue) => {
                    if(newValue !== null){
                      setCityName(newValue['id']);
                      setLoadedCity(newValue['label']);
                    }
                  }}
                  renderInput={(params) => <TextField {...params} 
                      variant="filled"
                      label="Search city" 
                      className="input"
                      onChange={handleSearch}
                  />}
                  isOptionEqualToValue={(option, newValue) => {
                    return option.id === newValue.id;
                  }}
                />
              </div>
            </Slide>

            <Slide direction="left" timeout={800} in={!loading}>
              <div className="">
                <CityName cityName={loadedCity} />
              </div>
            </Slide>

            <Slide direction="right" timeout={800} in={!loading}>
              <div className="">
                <WeatherToday 
                  summary={data['current']['summary']} 
                  icon={require(`./views/img/weather_icons/${data['current']['icon_num']}.png`)} 
                  temp={getTemp(data['current']['temperature'])}
                  date={formatDate('', 'today')}
                />
              </div>
            </Slide>

            <Slide direction="left" timeout={800} in={!loading}>
              <div className="">
                <Weather24Hours hourlyWeather={hourlyWeather} />
              </div>
            </Slide>

            <Slide direction="right" timeout={800} in={!loading}>
              <div className="">
                <Weather7Days dailyWeather={dailyWeather} />
              </div>
            </Slide>
          </>
        ) : (
          <CircularProgress/>
        )
      }
    </div>
  );
}

export default App;
