import { Circle, ElectroLight, Square, Triangle, MultiObject, getSeasonType, getShapes, getShapesStatic, getCanvasWinter } from './common.js';

let canvasDims
let canvasWidth;
let canvasHeight;

let shapesWinter = [];
let shapesWinterStatic = [];

/***  GET DATA from 'common.js' ***/
// get canvas
canvasDims = getCanvasWinter();
canvasWidth = canvasDims.width;
canvasHeight = canvasDims.height;

/***  TRANSFER DATA to 'common.js' ***/
// give shapes
getShapes(shapesWinter);
getShapesStatic(shapesWinterStatic);

// set season type
getSeasonType('Winter');

// check for right parent shape
export function checkParentShapeWinter(curShape, parentShape){ 
            // snowman
    return ((parentShape.index === 'snmBall1' && curShape.index === 'snmBall2') || (parentShape.index === 'snmBall2' && curShape.index === 'snmBall3') ||
            (parentShape.index === 'snmBall3' && curShape.group.includes('snmBall3Items')) ||
            // firTree
            (parentShape.index === 'firTree' && curShape.group.includes('firTreeItems')) ||
            // balls box 
            (parentShape.index === 'ballsBox' && curShape.group.includes('ballsBoxItems'))
            );
}

/***  SHAPES ***/
/**  Textures **/
// common
const texShadowRound = '../textures/shadow-round.png';
const texShadowSquare = '../textures/shadow-square.png';
const texDummy= '../winter/textures/dummy.png';

// sky
const texSky = '../winter/textures/sky.png';

// snow
const texSnow = '../winter/textures/snow.png';

// fir tree
const texFirTree = '../winter/textures/fir-tree.png';
const texFirTreeWire = '../winter/textures/fir-tree-wire.png';
const texFirTreeBall = '../winter/textures/fir-tree-ball.png';
const texFirTreeBallsBox = '../winter/textures/fir-tree-balls-box.png';
const texFirTreeStar = '../winter/textures/fir-tree-star.png';

// electro lights
const texSwitch = '../winter/textures/switch.png';
const texElectroLight = '../winter/textures/electro-light.png';
const texElectroLightBlue = '../winter/textures/electro-light-blue.png';
const texElectroLightRed = '../winter/textures/electro-light-red.png';
const texElectroLightGreen = '../winter/textures/electro-light-green.png';
const texElectroLightYellow = '../winter/textures/electro-light-yellow.png';

// snowman
const texSnmBall = '../winter/textures/snowman-ball.png';
const texSnmEye = '../winter/textures/snowman-eye.png';
const texSnmCarrot = '../winter/textures/carrot.png';
const texSnmBucket = '../winter/textures/bucket.png';
const texGiftBlue = '../winter/textures/gift-blue.png';
const texGiftGreen = '../winter/textures/gift-green.png';
const texGiftYellow = '../winter/textures/gift-yellow.png';

// Shape class - (index, xpos, ypos, image, imageShadow, isMovable, hasFreePosWhenChild, gravity, elasticity, friction)
const snowLevel = 115;
let surfaceLevel = canvasHeight - 30;

shapesWinter.push(new Square('sky', '', 0, 0, texSky, '', false, false, 0, 0, 0, canvasWidth, canvasHeight, false, 0, 0, 0, 0));
shapesWinter.push(new Square('snow', '', 0, canvasHeight-snowLevel, texSnow, '', false, false, 0, 0, 0, canvasWidth, snowLevel, false, 0, 0, 0, 0));
/*  FIR TREE */
let firTreeX = canvasWidth-550;
let firTreeY = canvasHeight-35;
shapesWinter.push(new Triangle('firTree', [], 0, 0, texFirTree, texShadowRound, false, false, 0, 0, 0, firTreeX, firTreeY, 270, -640, 500, 0));
shapesWinter.push(new Square('firTreeWire', [], firTreeX+85, firTreeY-600, texFirTreeWire, '', false, false, 0.0, 0.0, 0.0, 530, 600, false, 0, 0, 0, 0));
shapesWinter.push(new Square('switch', [], firTreeX+510, surfaceLevel-65, texSwitch, texShadowSquare, false, false, 0.1, 0.125, 0.03, 35, 50, false, 0, 0, 0, 0));

// ** electro lights
let redLight = new ElectroLight('elLight', 'elLights', firTreeX+420, firTreeY-80, texElectroLight, '', false, false, 0.0, 0.0, 0.0, 20, texElectroLightRed,       0,     0.25, 0.25);
let yellowLight = new ElectroLight('elLight', 'elLights', firTreeX+390, firTreeY-75, texElectroLight, '', false, false, 0.0, 0.0, 0.0, 20, texElectroLightYellow, 0.125, 0.25, 0.25);
let greenLight = new ElectroLight('elLight', 'elLights', firTreeX+360, firTreeY-65, texElectroLight, '', false, false, 0.0, 0.0, 0.0, 20, texElectroLightGreen,   0.25,  0.25, 0.25);
let blueLight = new ElectroLight('elLight', 'elLights', firTreeX+330, firTreeY-55, texElectroLight, '', false, false, 0.0, 0.0, 0.0, 20, texElectroLightBlue,     0.375, 0.25, 0.25);
let firTreeWire = new MultiObject(shapesWinter, 
[{x: firTreeX+420, y: firTreeY-80, obj: redLight}, {x: firTreeX+390, y: firTreeY-75, obj: yellowLight}, {x: firTreeX+360, y: firTreeY-65, obj: greenLight}, {x: firTreeX+330, y: firTreeY-55, obj: blueLight}, 
{x: firTreeX+300, y: firTreeY-50, obj: redLight}, {x: firTreeX+270, y: firTreeY-45, obj: yellowLight}, {x: firTreeX+240, y: firTreeY-45, obj: greenLight}, {x: firTreeX+210, y: firTreeY-50, obj: blueLight}, 
{x: firTreeX+180, y: firTreeY-57, obj: redLight}, {x: firTreeX+150, y: firTreeY-70, obj: yellowLight}, {x: firTreeX+120, y: firTreeY-85, obj: greenLight}, {x: firTreeX+95, y: firTreeY-105, obj: blueLight}, 
{x: firTreeX+87, y: firTreeY-135, obj: redLight}, {x: firTreeX+95, y: firTreeY-165, obj: yellowLight}, {x: firTreeX+120, y: firTreeY-185, obj: greenLight}, {x: firTreeX+150, y: firTreeY-190, obj: blueLight}, 
{x: firTreeX+180, y: firTreeY-190, obj: redLight}, {x: firTreeX+210, y: firTreeY-180, obj: yellowLight}, {x: firTreeX+240, y: firTreeY-165, obj: greenLight}, {x: firTreeX+270, y: firTreeY-150, obj: blueLight}, 
{x: firTreeX+305, y: firTreeY-145, obj: redLight}, {x: firTreeX+335, y: firTreeY-157, obj: yellowLight}, {x: firTreeX+360, y: firTreeY-177, obj: greenLight}, {x: firTreeX+392, y: firTreeY-177, obj: blueLight}, 
{x: firTreeX+417, y: firTreeY-190, obj: redLight}, {x: firTreeX+405, y: firTreeY-217, obj: yellowLight}, {x: firTreeX+397, y: firTreeY-247, obj: greenLight}, {x: firTreeX+380, y: firTreeY-270, obj: blueLight}, 
{x: firTreeX+350, y: firTreeY-280, obj: redLight}, {x: firTreeX+319, y: firTreeY-279, obj: yellowLight}, {x: firTreeX+295, y: firTreeY-260, obj: greenLight}, {x: firTreeX+266, y: firTreeY-247, obj: blueLight}, 
{x: firTreeX+234, y: firTreeY-247, obj: redLight}, {x: firTreeX+202, y: firTreeY-250, obj: yellowLight}, {x: firTreeX+171, y: firTreeY-258, obj: greenLight}, {x: firTreeX+156, y: firTreeY-284, obj: blueLight}, 
{x: firTreeX+163, y: firTreeY-313, obj: redLight}, {x: firTreeX+181, y: firTreeY-336, obj: yellowLight}, {x: firTreeX+207, y: firTreeY-351, obj: greenLight}, {x: firTreeX+237, y: firTreeY-355, obj: blueLight}, 
{x: firTreeX+267, y: firTreeY-355, obj: redLight}, {x: firTreeX+296, y: firTreeY-346, obj: yellowLight}, {x: firTreeX+325, y: firTreeY-340, obj: greenLight}, {x: firTreeX+354, y: firTreeY-347, obj: blueLight}, 
{x: firTreeX+357, y: firTreeY-375, obj: redLight}, {x: firTreeX+348, y: firTreeY-403, obj: yellowLight}, {x: firTreeX+319, y: firTreeY-412, obj: greenLight}, {x: firTreeX+288, y: firTreeY-408, obj: blueLight}, 
{x: firTreeX+256, y: firTreeY-402, obj: redLight}, {x: firTreeX+225, y: firTreeY-400, obj: yellowLight}, {x: firTreeX+203, y: firTreeY-416, obj: greenLight}, {x: firTreeX+208, y: firTreeY-445, obj: blueLight}, 
{x: firTreeX+227, y: firTreeY-469, obj: redLight}, {x: firTreeX+256, y: firTreeY-473, obj: yellowLight}, {x: firTreeX+286, y: firTreeY-464, obj: greenLight}, {x: firTreeX+316, y: firTreeY-459, obj: blueLight}, 
{x: firTreeX+330, y: firTreeY-483, obj: redLight}, {x: firTreeX+302, y: firTreeY-493, obj: yellowLight}, {x: firTreeX+270, y: firTreeY-496, obj: greenLight}, {x: firTreeX+239, y: firTreeY-501, obj: blueLight}, 
{x: firTreeX+226, y: firTreeY-525, obj: redLight}, {x: firTreeX+247, y: firTreeY-540, obj: yellowLight}, {x: firTreeX+275, y: firTreeY-540, obj: greenLight}, {x: firTreeX+295, y: firTreeY-556, obj: blueLight}, 
{x: firTreeX+284, y: firTreeY-581, obj: redLight}, {x: firTreeX+270, y: firTreeY-602, obj: yellowLight}]);
firTreeWire.draw();
// ** electro lights

// ** toys
shapesWinter.push(new Square('ballsBox', [], 20, 700, texFirTreeBallsBox, texShadowSquare, true, true, 0.5, 0.25, 0.03, 150, 100, true, 0, 10, 10, 10));
shapesWinter.push(new Circle('firTreeBall1', ['ballsBoxItems', 'firTreeItems'], 30, 600, texFirTreeBall, texShadowRound, true, false, 0.2, 0.2, 0.008, 25));
shapesWinter.push(new Circle('firTreeBall2', ['ballsBoxItems', 'firTreeItems'], 40, 600, texFirTreeBall, texShadowRound, true, false, 0.2, 0.2, 0.008, 25));
shapesWinter.push(new Circle('firTreeBall3', ['ballsBoxItems', 'firTreeItems'], 60, 600, texFirTreeBall, texShadowRound, true, false, 0.2, 0.2, 0.008, 25));
shapesWinter.push(new Circle('firTreeBall4', ['ballsBoxItems', 'firTreeItems'], 90, 600, texFirTreeBall, texShadowRound, true, false, 0.2, 0.2, 0.008, 25));
shapesWinter.push(new Circle('firTreeBall5', ['ballsBoxItems', 'firTreeItems'], 110, 600, texFirTreeBall, texShadowRound, true, false, 0.2, 0.2, 0.008, 25));
shapesWinter.push(new Circle('firTreeBall6', ['ballsBoxItems', 'firTreeItems'], 130, 600, texFirTreeBall, texShadowRound, true, false, 0.2, 0.2, 0.008, 25));
shapesWinter.push(new Circle('firTreeBall7', ['ballsBoxItems', 'firTreeItems'], 50, 600, texFirTreeBall, texShadowRound, true, false, 0.2, 0.2, 0.008, 25));
shapesWinter.push(new Circle('firTreeBall8', ['ballsBoxItems', 'firTreeItems'], 80, 600, texFirTreeBall, texShadowRound, true, false, 0.2, 0.2, 0.008, 25));
shapesWinter.push(new Circle('firTreeBall9', ['ballsBoxItems', 'firTreeItems'], 70, 600, texFirTreeBall, texShadowRound, true, false, 0.2, 0.2, 0.008, 25));
shapesWinter.push(new Square('star', ['ballsBoxItems', 'firTreeItems'], 140, 600, texFirTreeStar, texShadowRound, true, false, 0.1, 0.125, 0.03, 60, 60, false, 0, 0, 0, 0));
// ** toys

// ** snowman
shapesWinter.push(new Circle('snmBall1', [],  300, 200, texSnmBall,   texShadowRound, true, false, 0.2, 0.2, 0.008, 80));
shapesWinter.push(new Circle('snmBall2', [],  700, 200, texSnmBall,   texShadowRound, true, false, 0.2, 0.2, 0.008, 60));
shapesWinter.push(new Circle('snmBall3', [],  400, 200, texSnmBall,   texShadowRound, true, false, 0.2, 0.2, 0.008, 40));
shapesWinter.push(new Square('bucket', ['snmBall3Items'], 900, 200, texSnmBucket, texShadowRound, true, false, 0.3, 0.15, 0.04, 70, 70, false, 0, 0, 0, 0));
shapesWinter.push(new Circle('snmEye1', ['snmBall3Items'],  1000, 200, texSnmEye,    texShadowRound, true, false, 0.5, 0.125, 0.035, 9));
shapesWinter.push(new Circle('snmEye2', ['snmBall3Items'],  1370, 170, texSnmEye,    texShadowRound, true, false, 0.5, 0.125, 0.035, 9));
shapesWinter.push(new Square('carrot', ['snmBall3Items'], 1400, 220, texSnmCarrot, texShadowRound, true, false, 0.3, 0.15, 0.03, 60, 20, false, 0, 0, 0, 0));
// ** snowman

// gifts
shapesWinter.push(new Square('gift1', ['firTreeItems'], 700, 500, texGiftYellow, texShadowSquare, true, true, 0.1, 0.125, 0.03, 100, 100, false, 0, 0, 0, 0));
shapesWinter.push(new Square('gift2', ['firTreeItems'], 500, 500, texGiftBlue, texShadowSquare, true, true, 0.1, 0.125, 0.03, 80, 80, false, 0, 0, 0, 0));
shapesWinter.push(new Square('gift3', ['firTreeItems'], 250, 500, texGiftGreen, texShadowSquare, true, true, 0.1, 0.125, 0.03, 70, 70, false, 0, 0, 0, 0));

/**  NON ATTACHABLE **/
// snow
// shapesWinterStatic.push(new Square('snow', '', 0, canvasHeight-snowLevel, texSnow, '', false, false, 0.3, 0.15, 0.03, canvasWidth, snowLevel, false, 0, 0, 0, 0));

/*** FUNCTIONS ***/
function setAttachments(){
    // ** Balls box
    let ballBoxObject;
    // set children
    for(let shape of shapesWinter){
        if(shape.index === 'ballsBox'){
            ballBoxObject = shape;
            shape.isContainer = true;
            shape.children.push('firTreeBall1', 'firTreeBall2', 'firTreeBall3', 'firTreeBall4', 'firTreeBall5', 'firTreeBall6', 'star');
        }
    }
    // set parent
    for(let shape of shapesWinter){
        if(shape.group.includes('ballsBoxItems')){ 
            shape.parent = 'ballsBox'; shape.parentObject = ballBoxObject; shape.hasFreePosWhenChildOfContainer = true; 
            shape.deltaForAttachX = shape.xpos - shape.parentObject.xpos; 
            shape.deltaForAttachY = shape.ypos - shape.parentObject.ypos;
        }
    }

}
setAttachments();

/*** SPECIAL FUNCTIONS ***/
export function specialFunctionsWinter(shape){
    if(shape.index === 'switch'){ switchLights(); }
}

// electro lights
let switchedOn = false;
function switchLights(){
    if(switchedOn === false){
        switchedOn = true;
        for(let shape of shapesWinter){
            if(shape.group === 'elLights'){ shape.switchOnOff = true; }
        }
    } else {
        switchedOn = false;
        for(let shape of shapesWinter){
            if(shape.group === 'elLights'){ shape.switchOnOff = false; }
        }
    } 
}