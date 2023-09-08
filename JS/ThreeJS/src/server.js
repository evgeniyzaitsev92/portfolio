const http = require("http");

function createConeProps(props) {
    let triangles = [];
    for(let i = 0; i < props.N; i++){
        function setPoits(height){
            let triangle = [
                {1: props.R*Math.cos(2*Math.PI*i/props.N), 2: 0, 3: props.R*Math.sin(2*Math.PI*i/props.N)},
                {1: props.R*Math.cos(2*Math.PI*(i+1)/props.N), 2: 0, 3: props.R*Math.sin(2*Math.PI*(i+1)/props.N)},
                {1: 0, 2: height, 3: 0}
            ];
            triangles.push(triangle);
        }
        setPoits(props.H);
        setPoits(0);
    }
    return triangles;
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