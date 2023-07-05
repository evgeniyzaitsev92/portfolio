import { checkParentShapeWinter, specialFunctionsWinter } from "./winter.js";

/***  COMMON ***/
let seasonType;
export function getSeasonType(currentSeasonType){
    seasonType = currentSeasonType;
}

let canvas = document.getElementById('canvas');
let context = canvas.getContext('2d');
canvas.width = window.innerWidth;
canvas.height = window.innerHeight;
let surfaceLevel = canvas.height - 30;
let isDragging = false;
let curShapeIndex = null;
let startX;
let startY;
let mouseX;
let mouseY;
let offsetX;
let offsetY;
let isReadyToBeAttachedTo = null;

function getOffset(){
    let canvasOffsets = canvas.getBoundingClientRect();
    offsetX = canvasOffsets.left;
    offsetY = canvasOffsets.top;
}

getOffset();
window.onscroll = function(){getOffset();}
window.onresize = function(){
    canvas.width = window.innerWidth;
    canvas.height = window.innerHeight;
    getOffset();
}
canvas.onscroll = function(){getOffset();}

// send canvas
export function getCanvasWinter(){
    let canvasDims = {
        width: canvas.width,
        height: canvas.height
    }
    return canvasDims;
}


/**  OBJECTS **/
// isMovable
let shapes = [];
export function getShapes(gottenShapes){
    shapes = gottenShapes;
}

// static
let shapesStatic = [];
export function getShapesStatic(gottenShapes){
    shapesStatic = gottenShapes;
}

/*** FUNCTIONS ***/
/** COMMON **/
function lerp(p1, p2, t){
    return p1+(p2-p1)*t;
}

/** COLLISION **/
// hit canvas bounds
function shapeHitSurface(shape){
    let top, bottom, left, right, shapeHR /*heigt, or radius*/;

    // if circle
    if(shape.constructor.name == 'Circle'){
        top = shape.ypos - shape.radius;
        bottom = shape.ypos + shape.radius;
        left = shape.xpos - shape.radius;
        right = shape.xpos + shape.radius;
        shapeHR = shape.radius;

    // if square
    } else if(shape.constructor.name == 'Square'){
        top = shape.ypos;
        bottom = shape.ypos + shape.height;
        left = shape.xpos;
        right = shape.xpos + shape.width;
        shapeHR = shape.height;
    }

    if(shape.parentObject != null && shape.parentObject.isContainer==true && shape.hasFreePosWhenChildOfContainer == true){

        let paddingTop = shape.parentObject.ypos+shape.parentObject.paddingTop;
        let paddingBottom = shape.parentObject.ypos+shape.parentObject.height-shape.parentObject.paddingBottom;
        let paddingLeft = shape.parentObject.xpos+shape.parentObject.paddingLeft;
        let paddingRight = shape.parentObject.xpos+shape.parentObject.width-shape.parentObject.paddingRight;

        // top
        if(shape.parentObject.paddingTop != 0){
            if(top < paddingTop){ shape.ypos = paddingTop; shape.dy = shape.dy*shape.elasticity; shape.dy = 1; } 
        }

        // bottom
        if(bottom >= paddingBottom){ shape.ypos = paddingBottom-shapeHR; shape.dy = shape.dy*shape.elasticity; shape.dy *= -1; }

        // left
        if(left < paddingLeft){
            if(shape.constructor.name == 'Circle'){
                shape.xpos = shape.parentObject.xpos+shape.parentObject.paddingLeft+shape.radius;
            } else if(shape.constructor.name == 'Square'){
                shape.xpos = shape.parentObject.xpos+shape.parentObject.paddingLeft;
            }
            shape.dx *= -0.5; 
        }

        // right
        if(right >= paddingRight){
            if(shape.constructor.name == 'Circle'){
                shape.xpos = shape.parentObject.xpos+shape.parentObject.width-shape.parentObject.paddingRight-shape.radius;
            } else if(shape.constructor.name == 'Square'){
                shape.xpos = shape.parentObject.xpos+shape.parentObject.width-shape.parentObject.paddingRight-shape.width;
            }
            shape.dx *= -0.5; 
        }

    } else {
        // top
        if(top < 0){ shape.dy = shape.dy*shape.elasticity; shape.dy = 5; }  
        // bottom
        else if(bottom >= surfaceLevel){ shape.ypos = surfaceLevel-shapeHR; shape.dy = shape.dy*shape.elasticity; shape.dy *= -1; }
        // left
        else if(left < 0){ shape.dy = 3; shape.dx = 3; shape.move(); }        
        // right
        else if(right >= canvas.width){ shape.dy = 3; shape.dx = -3; shape.move(); }
    }

}

// collide circle and square
function collideCircleAndSquare(circle, square){
    function clamp(val, min, max) {
        return Math.max(min, Math.min(max, val))
    }
    
    // find the closest point to the circle within the rectangle
    let closestX = clamp(circle.xpos, square.xpos, square.xpos + square.width);
    let closestY = clamp(circle.ypos, square.ypos, square.ypos + square.height);
    
    // calculate the distance between the circle's center and this closest point
    let distanceX = circle.xpos - closestX;
    let distanceY = circle.ypos - closestY;
    
    // if the distance is less than the circle's radius, a collision occurs
    let distanceSquared = (distanceX * distanceX) + (distanceY * distanceY);
    return distanceSquared < (circle.radius * circle.radius);
}

// hit another shape
function isShapeHitted(shape, shapeForCollide){
    let collision = false;
    // if both circles
    if(shape.constructor.name == 'Circle' && shapeForCollide.constructor.name == 'Circle'){
        let dist = Math.sqrt(Math.pow((shape.xpos-shapeForCollide.xpos), 2) + Math.pow((shape.ypos-shapeForCollide.ypos), 2));
        collision = dist < (shape.radius+shapeForCollide.radius)*1.0;

    // if both squares
    } else if(shape.constructor.name == 'Square' && shapeForCollide.constructor.name == 'Square'){

    // if square and circle
    } else {
        collision = collideCircleAndSquare(shape, shapeForCollide);
    }
    return collision;
}

function shapeHitShape(shape){
    for(let shapeForCollide of shapes){
        if(shape.hasFreePosWhenChildOfContainer==true && shapeForCollide.hasFreePosWhenChildOfContainer==true && shape!=shapeForCollide){
            if(isShapeHitted(shape, shapeForCollide)){
                // bounce
                // if both circles
                if(shape.constructor.name == 'Circle' && shapeForCollide.constructor.name == 'Circle'){
                    if(shape.xpos<shapeForCollide.xpos){ 
                        shape.dx -= 0.1; 
                        shapeForCollide.dx += 0.1;
                        // shapeForCollide.dy = (shapeForCollide.dy * shapeForCollide.elasticity); shape.dy = (shape.dy * shape.elasticity);
                    } else { 
                        shape.dx += 0.1;
                        shapeForCollide.dx -= 0.1;
                        // shapeForCollide.dy = (shapeForCollide.dy * shapeForCollide.elasticity); shape.dy = (shape.dy * shape.elasticity);
                    }

                // if both squares
                } else if(shape.constructor.name == 'Square' && shapeForCollide.constructor.name == 'Square'){

                // if square and circle
                } else {
                    if(shape.xpos<shapeForCollide.xpos+shapeForCollide.width/2){ shape.dx -= 0.1; shapeForCollide.dx += 0.1; } else { shape.dx += 0.1; shapeForCollide.dx -= 0.1; }
                }
            }
        }
    }
}

// check if right parent
function checkIfRightParent(curShape, parentShape){
    let isRightParentShape = false;
    if(seasonType == 'Winter'){
        isRightParentShape = checkParentShapeWinter(curShape, parentShape);
    }

    return isRightParentShape;
}

// find objects for collision
function findForCollision(curShapeIndex){
    // for(let i = 0; i < shapes.length; i++){
    for(let i = shapes.length-1; i >= 0; i--){
        if(i == curShapeIndex){
            continue;
        }

        let allowAttach = false;
        // between circles
        if(shapes[i].constructor.name == 'Circle' && shapes[curShapeIndex].constructor.name == 'Circle'){
            let dist = Math.sqrt(Math.pow((shapes[i].xpos-shapes[curShapeIndex].xpos), 2) + Math.pow((shapes[i].ypos-shapes[curShapeIndex].ypos), 2));
            allowAttach = dist < (shapes[i].radius+shapes[curShapeIndex].radius);

        // between squares
        } else if(shapes[i].constructor.name == 'Square'  && shapes[curShapeIndex].constructor.name == 'Square'){
            allowAttach = !(shapes[curShapeIndex].ypos + shapes[curShapeIndex].height <= shapes[i].ypos ||
                shapes[curShapeIndex].ypos >= shapes[i].ypos + shapes[i].height ||
                shapes[curShapeIndex].xpos + shapes[curShapeIndex].width <= shapes[i].xpos ||
                shapes[curShapeIndex].xpos >= shapes[i].xpos + shapes[i].width);

        } else {
            let circle;
            let square;
            let trianle;

            // between circle and square
            if((shapes[i].constructor.name == 'Circle' && shapes[curShapeIndex].constructor.name == 'Square') || (shapes[i].constructor.name == 'Square' && shapes[curShapeIndex].constructor.name == 'Circle')){
                // get current shape
                if(shapes[i].constructor.name == 'Circle' && shapes[curShapeIndex].constructor.name == 'Square'){
                    circle = shapes[i];
                    square = shapes[curShapeIndex];
                } else if(shapes[i].constructor.name == 'Square' && shapes[curShapeIndex].constructor.name == 'Circle'){
                    square = shapes[i];
                    circle = shapes[curShapeIndex];
                }
                allowAttach = collideCircleAndSquare(circle, square);

            // triangle is always parent
            } else if(shapes[i].constructor.name == 'Triangle'){
                trianle = shapes[i];
                let isOnEdge = false;
                let triangleVX2 = trianle.xpos+trianle.vx2;
                let triangleVY2 = trianle.ypos+trianle.vy2;
                let triangleVX3 = trianle.xpos+trianle.vx3;
                let triangleVY3 = trianle.ypos+trianle.vy3;
                let edgeX1 = trianle.xpos - triangleVX2, edgeY1 = trianle.ypos - triangleVY2, edgeX2 = triangleVX2 - triangleVX3, edgeY2 = triangleVY2 - triangleVY3,
                edgeX3 = triangleVX3 - trianle.xpos, edgeY3 = triangleVY3 - trianle.ypos;

                // between triangle and circle
                if(shapes[curShapeIndex].constructor.name == 'Circle'){
                    circle = shapes[curShapeIndex];

                    let distanceX1 = trianle.xpos - circle.xpos, distanceY1 = trianle.ypos - circle.ypos, distanceX2 = triangleVX2 - circle.xpos,
                    distanceY2 = triangleVY2 - circle.ypos, distanceX3 = triangleVX3 - circle.xpos, distanceY3 = triangleVY3 - circle.ypos;
                    
                    let cathOnEdge1 = distanceX1*edgeX1 + distanceY1*edgeY1, cathOnEdge2 = distanceX2*edgeX2 + distanceY2*edgeY2, cathOnEdge3 = distanceX3*edgeX3 + distanceY3*edgeY3;
                    
                    if(cathOnEdge1 > 0){
                        let len = Math.sqrt(Math.pow(edgeX1, 2) + Math.pow(edgeY1, 2));
                        cathOnEdge1 = cathOnEdge1/len;

                        if((cathOnEdge1 < len)){
                            if(Math.sqrt(Math.pow(distanceX1, 2) + Math.pow(distanceY1, 2) - Math.pow(cathOnEdge1, 2)) <= circle.radius){
                                isOnEdge = true;
                            }
                        }                      
                    }
                    
                    if(cathOnEdge2 > 0){
                        let len = Math.sqrt(Math.pow(edgeX2, 2) + Math.pow(edgeY2, 2));
                        cathOnEdge2 = cathOnEdge2/len;

                        if((cathOnEdge2 < len)){
                            if(Math.sqrt(Math.pow(distanceX2, 2) + Math.pow(distanceY2, 2) - Math.pow(cathOnEdge2, 2)) <= circle.radius){
                                isOnEdge = true;
                            }
                        }                      
                    }

                    if(cathOnEdge3 > 0){
                        let len = Math.sqrt(Math.pow(edgeX3, 2) + Math.pow(edgeY3, 2));
                        cathOnEdge3 = cathOnEdge3/len;

                        if((cathOnEdge3 < len)){
                            if(Math.sqrt(Math.pow(distanceX3, 2) + Math.pow(distanceY3, 2) - Math.pow(cathOnEdge3, 2)) <= circle.radius){
                                isOnEdge = true;
                            }
                        }                      
                    }
                    
                    allowAttach =   // triangle corners
                                    (isPointInShape(trianle.xpos, trianle.ypos, circle) || isPointInShape(triangleVX2, triangleVY2, circle) || isPointInShape(triangleVX3, triangleVY3, circle) ||
                                    // circle center
                                    ((edgeY1*(circle.xpos - trianle.xpos) - edgeX1*(circle.ypos - trianle.ypos)) >= 0 && (edgeY2*(circle.xpos - triangleVX2) - edgeX2*(circle.ypos - triangleVY2)) >= 0 && (edgeY3*(circle.xpos - triangleVX3) - edgeX3*(circle.ypos - triangleVY3)) >= 0) ||
                                    // collide edges
                                    isOnEdge);

                } else if(shapes[curShapeIndex].constructor.name == 'Square'){
                    square = shapes[curShapeIndex];
                    let top = square.ypos;
                    let bottom = square.ypos + square.height;
                    let left = square.xpos;
                    let right = square.xpos + square.width;

                    let trV1 = { x: trianle.xpos, y: trianle.ypos}, trV2 = { x: triangleVX2, y: triangleVY2}, trV3 = { x: triangleVX3, y: triangleVY3};
                    let sqV1 = { x: left, y: top}, sqV2 = { x: right, y: top}, sqV3 = { x: right, y: bottom}, sqV4 = { x: left, y: bottom};

                    // function lerp(p1, p2, t){return p1+(p2-p1)*t;}

                    function getIntersection(p1, p2, p3, p4){
                        let formulaTTop = (p4.x-p3.x)*(p1.y-p3.y) - (p4.y-p3.y)*(p1.x-p3.x);
                        let formulaUTop = (p3.y-p1.y)*(p1.x-p2.x) - (p3.x-p1.x)*(p1.y-p2.y);
                        let formulaBottom = (p4.y-p3.y)*(p2.x-p1.x) - (p4.x-p3.x)*(p2.y-p1.y);
                        
                        if(formulaBottom != 0){
                            let t = formulaTTop/formulaBottom;
                            let u = formulaUTop/formulaBottom;
                            if(t>=0 && t<=1 && u>=0 && u<=1){
                                return true;
                            }

                        }
                        return false;
                    }

                    isOnEdge =  // 1st triangle edge and square edges
                                getIntersection(trV1, trV2, sqV1, sqV2) || getIntersection(trV1, trV2, sqV2, sqV3) || getIntersection(trV1, trV2, sqV3, sqV4) || getIntersection(trV1, trV2, sqV4, sqV1) ||
                                // 2st triangle edge and square edges
                                getIntersection(trV2, trV3, sqV1, sqV2) || getIntersection(trV2, trV3, sqV2, sqV3) || getIntersection(trV2, trV3, sqV3, sqV4) || getIntersection(trV2, trV3, sqV4, sqV1) ||
                                // 3st triangle edge and square edges
                                getIntersection(trV3, trV1, sqV1, sqV2) || getIntersection(trV3, trV1, sqV2, sqV3) || getIntersection(trV3, trV1, sqV3, sqV4) || getIntersection(trV3, trV1, sqV4, sqV1);
                                    
                    allowAttach =   // triangle corners inside rectangle
                                    (isPointInShape(trianle.xpos, trianle.ypos, square) || isPointInShape(triangleVX2, triangleVY2, square) || isPointInShape(triangleVX3, triangleVY3, square) ||
                                    // rectangle corners inside triangle
                                    ((edgeY1*(left - trianle.xpos) - edgeX1*(top - trianle.ypos)) >= 0 && (edgeY2*(left - triangleVX2) - edgeX2*(top - triangleVY2)) >= 0 && (edgeY3*(left - triangleVX3) - edgeX3*(top - triangleVY3)) >= 0) ||
                                    ((edgeY1*(right - trianle.xpos) - edgeX1*(top - trianle.ypos)) >= 0 && (edgeY2*(right - triangleVX2) - edgeX2*(top - triangleVY2)) >= 0 && (edgeY3*(right - triangleVX3) - edgeX3*(top - triangleVY3)) >= 0) ||
                                    ((edgeY1*(left - trianle.xpos) - edgeX1*(bottom - trianle.ypos)) >= 0 && (edgeY2*(left - triangleVX2) - edgeX2*(bottom - triangleVY2)) >= 0&&(edgeY3*(left - triangleVX3) - edgeX3*(bottom - triangleVY3)) >= 0) ||
                                    ((edgeY1*(right - trianle.xpos) - edgeX1*(bottom - trianle.ypos)) >= 0 && (edgeY2*(right - triangleVX2) - edgeX2*(bottom - triangleVY2)) >= 0 && (edgeY3*(right - triangleVX3) - edgeX3*(bottom - triangleVY3)) >= 0) ||
                                    // collide edges
                                    isOnEdge);
                }
            }
        }

        if(allowAttach){
            if(!shapes[curShapeIndex].children.includes(shapes[i].index) && checkIfRightParent(shapes[curShapeIndex], shapes[i])){
                isReadyToBeAttachedTo = shapes[i].index;
                shapes[curShapeIndex].preAttachState = true;
                break;
            }
        } else {
            isReadyToBeAttachedTo = null;
            shapes[curShapeIndex].preAttachState = false;
            shapes[curShapeIndex].parent = null;
            shapes[curShapeIndex].parentObject = null;
            shapes[curShapeIndex].hasFreePosWhenChildOfContainer = false;
            for(let shape of shapes){
                if(shape.children.includes(shapes[curShapeIndex].index)){
                    let idxToRemove = shape.children.indexOf(shapes[curShapeIndex].index)
                    shape.children.splice(idxToRemove, 1);
                }
            }
        } 
    }
}

/** ATTACHMENTS **/
function findForAttachments(shape){
    if(shape.parentObject != null){
        // when child object not pressed by mouse
        if(shape.allowMoveWhenHasParent == false && shape.hasFreePosWhenChild==false && shape.hasFreePosWhenChildOfContainer==false){
            shape.xpos = shape.parentObject.xpos + shape.deltaForAttachX;
            shape.ypos = shape.parentObject.ypos + shape.deltaForAttachY;
        } else if(shape.hasFreePosWhenChildOfContainer==true){
            shape.xpos = lerp(shape.xpos, shape.parentObject.xpos + shape.deltaForAttachX, 0.25);
        }
    }
}

/** MOVEMENTS **/
// if mouse cursor in shape
function isPointInShape(x, y, shape){
    // if circle
    if(shape.constructor.name == 'Circle'){
        let dist = Math.sqrt(Math.pow((x-shape.xpos), 2) + Math.pow((y-shape.ypos), 2));
        if(dist < shape.radius){ return true; }
    
    // if square
    } else if(shape.constructor.name == 'Square'){
        let shapeLeft = shape.xpos;
        let shapeRight = shape.xpos + shape.width;
        let shapeTop = shape.ypos;
        let shapeBottom = shape.ypos + shape.height;
                // left           // right                          // top             // bottom
        if(x > shape.xpos && x < (shape.xpos + shape.width) && y > shape.ypos && y < (shape.ypos + shape.height)){ return true; }
    }
    return false;
}

// press mouse
function mouseDown(event){
    event.preventDefault();

    // mouse start position
    startX = parseInt(event.clientX - offsetX);
    startY = parseInt(event.clientY - offsetY);

    // loop from the end to the upper element
    for(let i = shapes.length-1; i >= 0; i--){
        // if mouse within object bounds
        if(isPointInShape(startX, startY, shapes[i])){
            shapes[i].dx = 0;
            shapes[i].dy = 0;
            curShapeIndex = i;
            if(shapes[i].isMovable == true){
                isDragging = true;
            }

            // if current object has parent, allow move freely
            if(shapes[curShapeIndex].parent != null){
                shapes[curShapeIndex].allowMoveWhenHasParent = true;
            }
            break;
        }
    }
    
}

// release mouse
function mouseUp(event){
    // if(!isDragging){
    //     return;
    // }

    event.preventDefault(); 
    isDragging = false;
    // if there is an object to be attached to
    if(isReadyToBeAttachedTo != null){
        let isToBeAttachedTo = isReadyToBeAttachedTo;
        isReadyToBeAttachedTo = null;
        
        // to make parent and obtain a child
        for(let shape of shapes){
            if(shape.index == isToBeAttachedTo){
                // if object does not have current object in its children list and current object does not already have a parent, push it in the list
                if(!shape.children.includes(shapes[curShapeIndex].index) && shapes[curShapeIndex].parent == null){
                    shape.children.push(shapes[curShapeIndex].index);
                }
                
                // if current object children list does not contain a possible objest for attaching, make this object parent of current object
                if(!shapes[curShapeIndex].children.includes(isToBeAttachedTo)){          
                    shapes[curShapeIndex].parent = isToBeAttachedTo;
                    shapes[curShapeIndex].parentObject = shape;
                    if(shapes[curShapeIndex].parentObject.isContainer==true){
                        shapes[curShapeIndex].hasFreePosWhenChildOfContainer = true;
                    }

                    // set position reative to parent object
                    shapes[curShapeIndex].deltaForAttachX = shapes[curShapeIndex].xpos - shape.xpos;
                    shapes[curShapeIndex].deltaForAttachY = shapes[curShapeIndex].ypos - shape.ypos;
                }
                shapes[curShapeIndex].preAttachState = false;
                break; 
            }
        }
    } 
    // SPECIAL FUNCTIONS
    if(seasonType == 'Winter'){
        specialFunctionsWinter(shapes[curShapeIndex]);
    }
    shapes[curShapeIndex].allowMoveWhenHasParent = false;
    curShapeIndex = null;
}

// mouse outside canvas 
function mouseOut(event){
    if(!isDragging){
        return;
    } 
    
    event.preventDefault();
    isDragging = false;
    curShapeIndex = null;
}

// move mouse
function mouseMove (event){
    if(!isDragging){
        return;
    } else {
        event.preventDefault();
        mouseX = parseInt(event.clientX - offsetX);
        mouseY = parseInt(event.clientY - offsetY);
        let dx = mouseX - startX;
        let dy = mouseY - startY;
        let curShape = shapes[curShapeIndex];
        curShape.xpos += dx;
        curShape.ypos += dy;
        startX = mouseX;
        startY = mouseY;
        findForCollision(curShapeIndex);
    }
}

canvas.onmousedown = mouseDown;
canvas.onmouseup = mouseUp;
canvas.onmouseout = mouseOut;
canvas.onmousemove = mouseMove;

/*** MAIN ***/
function main(){
    requestAnimationFrame(main);
    context.clearRect(0, 0, canvas.width, canvas.height);

    // static shapes
    for(let i = 0; i < shapesStatic.length; i++){
        shapesStatic[i].draw();
    }

    // isMovable shapes
    for(let i = 0; i < shapes.length; i++){
        if(i != curShapeIndex){
            findForAttachments(shapes[i]);
            shapes[i].move();
            if(shapes[i].isMovable == true){ shapeHitSurface(shapes[i]); }
            // shapeHitShape(shapes[i]);
            shapes[i].draw();
        } else {
            shapes[i].draw();
            continue;
        }
    }
}
main();
/*** MAIN ***/

/*** CLASSES ***/
class Shape {
    constructor(index, group, xpos, ypos, image, imageShadow, isMovable, hasFreePosWhenChild, gravity, elasticity, friction){
        this.index = index;
        this.group = group;
        this.xpos = xpos;
        this.ypos = ypos;
        this.image = image;
        this.imageShadow = imageShadow;
        this.isMovable = isMovable;
        this.hasFreePosWhenChild = hasFreePosWhenChild;
        this.hasFreePosWhenChildOfContainer = false;
        this.dx = 0;
        this.dy = 0; // dy; // 5;
        this.gravity = gravity; // 0.1;
        this.elasticity = elasticity; // 0.25;
        this.friction = friction; // 0.008;
        this.parent = null;
        this.parentObject = null;
        this.children = [];
        this.allowMoveWhenHasParent = false;
        this.deltaForAttachX = null;
        this.deltaForAttachY = null;
        this.preAttachState = false;
        this.image = new Image();
        this.image.src = image;
        this.imageShadow = new Image();
        this.imageShadow.src = imageShadow;
        this.image.onload = () => {
            this.draw()
        };
    }

    move(){
        // if has no parent
        if((this.parent == null && this.isMovable == true) || this.hasFreePosWhenChild == true || this.hasFreePosWhenChildOfContainer == true){
            let surface;
            if(this.hasFreePosWhenChildOfContainer == true){
                surface = this.parentObject.ypos+this.parentObject.height-this.parentObject.paddingBottom;
            } else {
                surface = canvas.height;
            }

            // apply gravity
            if(this.ypos + this.gravity < surface){
                this.dy += this.gravity;
            }
            // apply speed
            this.ypos += this.dy;
    
            // apply friction
            if(this.hasFreePosWhenChildOfContainer == true){
                this.dx *= 0.975;
            }
            this.dx = this.dx - (this.dx*this.friction)
            this.xpos += this.dx;
        }
    }
}

export class Circle extends Shape {
    constructor(index, group, xpos, ypos, image, imageShadow, isMovable, hasFreePosWhenChild, gravity, elasticity, friction, radius){
        super(index, group, xpos, ypos, image, imageShadow, isMovable, hasFreePosWhenChild, gravity, elasticity, friction);
        this.radius = radius;
    }

    draw(){
        // shadow
        if((this.parent == null && this.preAttachState == false) || this.hasFreePosWhenChild == true){
            let shadowWidth = (this.radius*3)*((this.ypos+this.radius)/surfaceLevel);
            let shadowHeight = shadowWidth/4;
            context.drawImage(this.imageShadow, this.xpos-this.radius, surfaceLevel-shadowHeight/2, shadowWidth, shadowHeight);
        }

        // draw image
        context.beginPath();
        context.arc(this.xpos, this.ypos, this.radius, 0, Math.PI*2, false);
        context.closePath();
        context.drawImage(this.image, this.xpos-this.radius, this.ypos-this.radius, this.radius*2, this.radius*2);
        if(this.preAttachState == true){
            context.strokeStyle = 'rgb(36, 217, 0)';
            context.lineWidth = 6; 
            context.stroke();
        }
    }
}

export class ElectroLight extends Circle {
    constructor(index, group, xpos, ypos, image, imageShadow, isMovable, hasFreePosWhenChild, gravity, elasticity, friction, radius, imageLight, startDelay, waitDelayOn, waitDelayOff){
        super(index, group, xpos, ypos, image, imageShadow, isMovable, hasFreePosWhenChild, gravity, elasticity, friction, radius);
        this.imageLight = new Image();
        this.imageLight.src = imageLight;
        this.switchOnOff = false;
        this.isOn = false;
        this.startDelay = startDelay*1000;
        this.waitDelayOn = waitDelayOn*1000;
        this.waitDelayOff = waitDelayOff*1000;
        this.timeStart = 0;
        this.timeWaitOn = 0;
        this.timeWaitOff= 0;
        this.allowStartDelay = true;
    }

    draw(){
        // shadow
        if((this.parent == null && this.preAttachState == false) || this.hasFreePosWhenChild == true){
            let shadowWidth = (this.radius*3)*((this.ypos+this.radius)/surfaceLevel);
            let shadowHeight = shadowWidth/4;
            context.drawImage(this.imageShadow, this.xpos-this.radius, surfaceLevel-shadowHeight/2, shadowWidth, shadowHeight);
        }

        // draw image
        context.beginPath();
        context.arc(this.xpos, this.ypos, this.radius, 0, Math.PI*2, false);
        context.closePath();
        context.drawImage(this.image, this.xpos-this.radius, this.ypos-this.radius, this.radius*2, this.radius*2);

        if(this.switchOnOff == true){
            if(this.allowStartDelay == true){
                this.allowStartDelay = false;
                this.timeStart = Date.now() + this.startDelay;
                this.timeWaitOn = this.timeStart + this.waitDelayOn;
                this.isOn = true;
                // this.timeWaitOff = this.timeWaitOn + this.waitDelayOff;
            }
            
            if(Date.now() >= this.timeStart){
                if(this.isOn == true){
                    if(Date.now() <= this.timeWaitOn){
                        context.drawImage(this.imageLight, this.xpos-this.radius, this.ypos-this.radius, this.radius*2, this.radius*2);
                    } else {
                        this.isOn = false;
                        this.timeWaitOff = this.timeWaitOn + this.waitDelayOff;
                    }
                } else {
                    if(Date.now() >= this.timeWaitOff){
                        this.isOn = true;
                        this.timeWaitOn = this.timeWaitOff + this.waitDelayOn;
                    }
                }
            }
        } else {
            this.isOn = false;
            this.allowStartDelay = true;
            this.timeStart = 0;
            this.timeWaitOn = 0;
            this.timeWaitOff= 0;
        }
         
        // if(this.preAttachState == true){
        //     context.strokeStyle = 'rgb(36, 217, 0)';
        //     context.lineWidth = 6; 
        //     context.stroke();
        // }
    }
}

export class Square extends Shape {
    constructor(index, group, xpos, ypos, image, imageShadow, isMovable, hasFreePosWhenChild, gravity, elasticity, friction, width, height, 
                isContainer, paddingTop, paddingBottom, paddingLeft, paddingRight){
        super(index, group, xpos, ypos, image, imageShadow, isMovable, hasFreePosWhenChild, gravity, elasticity, friction);
        this.width = width;
        this.height = height;
        this.isContainer = isContainer;
        this.paddingTop = paddingTop;
        this.paddingBottom = paddingBottom;
        this.paddingLeft = paddingLeft;
        this.paddingRight = paddingRight;
    }

    draw(){
        // shadow
        if((this.parent == null && this.preAttachState == false) || this.hasFreePosWhenChild == true){
            let shadowWidth;
            this.isMovable ? shadowWidth = ((this.width*1.25)*((this.ypos+this.height)/surfaceLevel)) : shadowWidth = (this.width*1.25);
            let shadowHeight = this.height/2*(this.ypos/surfaceLevel);
            context.drawImage(this.imageShadow, this.xpos, surfaceLevel-shadowHeight/2, shadowWidth, shadowHeight);
        }

        // draw image
        context.beginPath(); 
        context.drawImage(this.image, this.xpos, this.ypos, this.width, this.height);
        context.closePath();
        if(this.preAttachState == true){
            context.strokeStyle = 'rgb(36, 217, 0)';
            context.lineWidth = 4; 
            context.strokeRect(this.xpos, this.ypos, this.width, this.height);
        }
    }
}

export class Triangle extends Shape {
    constructor(index, group, xpos, ypos, image, imageShadow, isMovable, hasFreePosWhenChild, gravity, elasticity, friction, vx1Start, vy1Start, vx2Plus, vy2Plus, vx3Plus, vy3Plus){
        super(index, group, xpos, ypos, image, imageShadow, isMovable, hasFreePosWhenChild, gravity, elasticity, friction);
        this.xpos = vx1Start;
        this.ypos = vy1Start;
        this.vx2 = vx2Plus;
        this.vy2 = vy2Plus;
        this.vx3 = vx3Plus;
        this.vy3 = vy3Plus;
        this.minX = 0;
        this.maxX = 0;
    }

    draw(){
        this.minX = Math.min(this.xpos, this.xpos+this.vx2, this.xpos+this.vx3);
        this.maxX = Math.max(this.xpos, this.xpos+this.vx2, this.xpos+this.vx3);
        this.minY = Math.min(this.ypos, this.ypos+this.vy2, this.ypos+this.vy3);
        this.maxY = Math.max(this.ypos, this.ypos+this.vy2, this.ypos+this.vy3);
        let imageWidth = this.maxX-this.minX;
        let imageHeight = this.maxY-this.minY;

        // shadow
        if((this.parent == null && this.preAttachState == false) || this.hasFreePosWhenChild == true){
            let shadowWidth;
            let shadowHeight;
            this.isMovable ? shadowWidth = ((imageWidth*1.25)*((this.ypos+imageHeight)/surfaceLevel)) : shadowWidth = (imageWidth*1.25);
            this.isMovable ? shadowHeight = imageHeight/4*(this.ypos/surfaceLevel) : shadowHeight = imageHeight/6*(this.ypos/surfaceLevel);
            context.drawImage(this.imageShadow, this.xpos, surfaceLevel-shadowHeight/2, shadowWidth, shadowHeight);
        }

        // draw image
        context.beginPath();
        context.moveTo(this.xpos, this.ypos);
        context.lineTo(this.xpos+this.vx2, this.ypos+this.vy2);
        context.lineTo(this.xpos+this.vx3, this.ypos+this.vy3);
        context.closePath();
        // context.stroke();
        context.drawImage(this.image, this.xpos, this.ypos-imageHeight, imageWidth, imageHeight);
        //     context.strokeStyle = 'rgb(36, 217, 0)';
        //     context.lineWidth = 6; 
        //     context.stroke();
        // }
    }
}

export class MultiObject {
    constructor(shapesArrRef, coordsArr){ // coordsArr = [{x: 0, y: 0, obj: null}]
        this.shapesArrRef = shapesArrRef;
        this.coordsArr = coordsArr;
        this.objects = {};
    }

    draw(){
        for(let i=0; i<this.coordsArr.length; i++){
            // object = this.coordsArr[i].obj;
            let object = Object.assign(Object.create(Object.getPrototypeOf(this.coordsArr[i].obj)), this.coordsArr[i].obj);
            object.xpos = this.coordsArr[i].x;
            object.ypos = this.coordsArr[i].y;
            this.shapesArrRef.push(object);
        }
    }
}