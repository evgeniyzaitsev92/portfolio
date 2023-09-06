const http = require("http");
const THREE = require('three');
import main from './main.js';

// Build cone 
function createTriangles(props) {
    let triangles = [];
    for(let i = 0; i < props.N; i++){
        
        function buildTriangle(height){
            let points = [];
            points.push(new THREE.Vector3(props.R*Math.cos(2*Math.PI*i/props.N), 0, props.R*Math.sin(2*Math.PI*i/props.N)));
            points.push(new THREE.Vector3(props.R*Math.cos(2*Math.PI*(i+1)/props.N), 0, props.R*Math.sin(2*Math.PI*(i+1)/props.N)));
            points.push(new THREE.Vector3(0, height, 0));
            let geometry = new THREE.BufferGeometry().setFromPoints(points);
        
            if(props.hasFaces == true){
                const triangleMaterial = new THREE.MeshPhongMaterial({
                    color: 0xfc0303,
                    side: THREE.DoubleSide, 
                    flatShading: true
                });
                var triangleMesh = new THREE.Mesh(geometry, triangleMaterial);
                triangleMesh.castShadow = true; //default is false
                triangleMesh.receiveShadow = false;
                triangles.push(triangleMesh);
            }
    
            if(props.hasWireFrame == true){
                var line = new THREE.Line(geometry, new THREE.LineBasicMaterial({ color: 0x09ff00 }));
                triangles.push(line);
            }
        }
        buildTriangle(props.H);
        buildTriangle(0);
    }
    return triangles;
};

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
        main.update(createTriangles(data['props'])); // update cone
    });
};

const server = http.createServer(requestListener8000);
server.listen(port, host, () => {
    console.log(`Server is running on http://${host}:${port}`);
    main.update(createTriangles(main.props)); // build cone on start
});