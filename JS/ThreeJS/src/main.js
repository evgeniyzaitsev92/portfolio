const http = require("http");
const THREE = require('three');
const gui = new dat.GUI();
const view = document.getElementById('view');
const REDUCER = 20;

// Scene
const scene = new THREE.Scene();
scene.background = new THREE.Color(0x1168f5);

// Camera
const camera = new THREE.PerspectiveCamera(100, (window.innerWidth-REDUCER)/(window.innerHeight-REDUCER), 0.1, 1000);
camera.position.set(0, 1, 3);
const camFolder = gui.addFolder('Camera');
camFolder.add(camera.position, 'x', -10, 10, 0.25).name('Move X');
camFolder.add(camera.position, 'y', -10, 10, 0.25).name('Move Y');
camFolder.add(camera.position, 'z', -10, 10, 0.25).name('Move Z');
const angle =  Math.PI/180; 
camFolder.add(camera.rotation, 'x', 0, 360*angle, 1*angle).name('Rotate X');
camFolder.add(camera.rotation, 'y', 0, 360*angle, 1*angle).name('Rotate Y');
camFolder.add(camera.rotation, 'z', 0, 360*angle, 1*angle).name('Rotate Z');
camFolder.open();

// Renderer
const renderer = new THREE.WebGLRenderer();
renderer.shadowMap.enabled = true;
renderer.shadowMap.type = THREE.PCFSoftShadowMap;
view.appendChild(renderer.domElement);

// Light
const lightAmb = new THREE.AmbientLight(0x8c8c8c);
scene.add(lightAmb);

const light = new THREE.PointLight(0xffffff, 75);
light.position.set(4, 4, 4);
light.castShadow = true;
light.shadow.normalBias = 0.5;
scene.add(light);

// Properties
let props = {
    R: 1,
    N: 6,
    H: 2,
    hasFaces: true,
    hasWireFrame: false,
};

const propsFolder = gui.addFolder('Properties');
propsFolder.add(props, 'R', 0, 16, 0.25).name('Radius').onChange(function(val){
    props.R = val;
    sendToServer(props);
});
propsFolder.add(props, 'N', 3, 36, 1).name('Num of faces').onChange(function(val){
    props.N = val;
    sendToServer(props);
});
propsFolder.add(props, 'H', 0, 16, 0.25).name('Height').onChange(function(val){
    props.H = val;
    sendToServer(props);
});
propsFolder.add(props, 'hasFaces').name('Faces').onChange(function(val){
    props.hasFaces = val;
    sendToServer(props);
});
propsFolder.add(props, 'hasWireFrame').name('Wireframe').onChange(function(val){
    props.hasWireFrame = val;
    sendToServer(props);
});
propsFolder.open();

// Triangle
let triangles = [];
function update(elements){
    triangles.forEach(element => {
        scene.remove(element); 
    });

    elements.forEach(element => {
        scene.add(element);
        triangles.push(element);
    });
}

// Plane
const planeGeometry = new THREE.PlaneGeometry(20, 20);
const plane = new THREE.Mesh(planeGeometry, new THREE.MeshPhongMaterial({ color: 0x03fc66, flatShading: true }));
plane.rotateX(-Math.PI / 2);
plane.position.y = -1.75;
plane.receiveShadow = true;
scene.add(plane);

// Cone manupulation
let mouseDown = false, mouseX = 0, mouseY = 0;

function onMouseMove(event){
    if(!mouseDown){
        return;
    }

    event.preventDefault();

    let deltaX = event.clientX - mouseX;
    let deltaY = event.clientY - mouseY;
    mouseX = event.clientX;
    mouseY = event.clientY;
    rotateScene(deltaX, deltaY);
}

function onMouseDown(event){
    event.preventDefault();
    mouseDown = true;
    mouseX = event.clientX;
    mouseY = event.clientY;
}

function onMouseUp(event){
    event.preventDefault();
    mouseDown = false;
}

function addMouseHandler(view){
    view.addEventListener('mousemove', function(event){
        onMouseMove(event);
    }, false);
    view.addEventListener('mousedown', function(event){
        onMouseDown(event);
    }, false);
    view.addEventListener('mouseup', function(event){
        onMouseUp(event);
    }, false);
}

function rotateScene(deltaX, deltaY){
    triangles.forEach(element => {
        element.rotation.y += deltaX / 100;
        element.rotation.z += deltaY / 100;
    });
}
addMouseHandler(view);

// Send info to server
function sendToServer(val){
    const postData = JSON.stringify({props: val});

    const postOptions = {
        host: 'localhost',
        port: '8000',
        path: '/',
        method: 'POST',
        headers: {
            'Content-Type': 'application/json',
            'Content-Length': Buffer.byteLength(postData)
        }
    };

    const postReq = http.request(postOptions, function(res){
        res.setEncoding('utf8');
        res.on('data', function(chunk){});
    });
  
    postReq.write(postData);
    postReq.end();
};

// Resize window
window.addEventListener('resize', onWindowResize, false);
function onWindowResize(){
    camera.aspect = (window.innerWidth-REDUCER)/(window.innerHeight-REDUCER);
    camera.updateProjectionMatrix();
    renderer.setSize((window.innerWidth-REDUCER), window.innerHeight-REDUCER);
}

// Render
function render(){
    requestAnimationFrame(render);   
    renderer.render(scene, camera);
    renderer.setClearColor(0x000000, 0.0);
}
render();

export default{
    update, props
}