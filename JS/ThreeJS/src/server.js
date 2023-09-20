const http = require("http");

function createConeProps(props) {
    let points = [];
    for(let i = 0; i < props.N; i++){
        let point = {'x': props.R*Math.cos(2*Math.PI*i/props.N), 'z': props.R*Math.sin(2*Math.PI*i/props.N)};
        points.push(point);
    }
    return points;
}

// SERVER
const host = 'localhost';
const port = 8000;
const requestListener8000 = function (req, res) {
    let body = [];
    req.on('data', (chunk) => {
        body.push(chunk);
    }).on('end', () => {
        body = Buffer.concat(body).toString();
        const data = JSON.parse(body);
        const pointsData = createConeProps(data['props']);
        res.end(JSON.stringify(pointsData));
    });
};

const server = http.createServer(requestListener8000);
server.listen(port, host, () => {
    console.log(`Server is running on http://${host}:${port}`);
});