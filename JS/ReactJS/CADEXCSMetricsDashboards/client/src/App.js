import React, {useEffect, useState, useRef} from 'react';
import { CircularProgress, Select, MenuItem } from '@mui/material';
import './App.css';
import {TableComponent} from './components/TableComponent.js'

function App() {
  const [slaData, setSLAData] = useState();
  const [slaPeriod, setSLAPeriod] = useState('week');
  const didMount = useRef(false);

  useEffect(() => {
    if (didMount.current) {
      didMount.current = false;
      fetch(`http://localhost:5000/sla/${slaPeriod}`)
      .then(res => res.json())
      .then(data => {
          // console.log(data);
          setSLAData(data);
        }
      )
    } else {
      didMount.current = true;
    }
  }, [slaData, slaPeriod]);

  function updateMetrics(event, metric){
    if(metric === 'SLA'){
      setSLAPeriod(event['target']['value']);
      setSLAData(undefined);
    }
  };

  function selectPeriod(metric){
    return (
      <Select
        // value={'week'}
        defaultValue={'week'}
        onChange={(event) => updateMetrics(event, metric)}
        sx={{
          backgroundColor: 'white',
          width: 130,
          height: 30,
        }}
      >
        <MenuItem value={'week'}>Week</MenuItem>
        <MenuItem value={'two_weeks'}>Two weeks</MenuItem>
        <MenuItem value={'month'}>Month</MenuItem>
      </Select>
    );
  };
  
  return (
    <div className="container">
      <h1>Dashboards</h1>
      <div className='metrics d-grid'>
        {/* SLA */}
        <div className='container-metric d-flex flex-dir-col ai-cntr'>
          <h2 className='clr-white'>SLA Violations</h2>
          {selectPeriod('SLA')}
          {
            (slaData === undefined) ? (
              <CircularProgress style={{'color': 'white'}}/>
            ) : (
              <TableComponent metric='SLA' data={slaData} />
            )
          }
        </div>

        <div className='container-metric'>
          
        </div>

        <div className='container-metric'>
          
        </div>
      </div>
    </div>
  );
}

export default App;