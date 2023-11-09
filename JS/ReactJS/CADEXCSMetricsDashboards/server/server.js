const express = require('express');
const app = express();
const cors = require('cors');
const main = require('./main.js');

app.use(cors());
app.get('/sla/:period', (req, res) => {
    // console.log('app', req.params.period);
    (async function() {
        res.json(await main.fetchData('SLA', req.params.period));
    })();
});

app.listen(5000, () => {console.log('Server started on port 5000...')});