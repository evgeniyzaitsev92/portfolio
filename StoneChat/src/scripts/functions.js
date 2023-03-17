function inputHostPort(){
    let hasPortHost;
    if(document.getElementById("host").value === "" || document.getElementById("port").value === ""){
        document.getElementById("btn-run").disabled = true;
        hasPortHost = false;
    } else {
        document.getElementById("btn-run").disabled = false;
        hasPortHost = true;
    }

    if(document.getElementById("client_title").value === "" || !hasPortHost){
        document.getElementById("btn-connect").disabled = true;
    } else {
        document.getElementById("btn-connect").disabled = false;
    }
}

document.getElementById("host").oninput = function(){
    inputHostPort();
};

document.getElementById("port").oninput = function(){
    inputHostPort();
};

document.getElementById("client_title").oninput = function(){
    inputHostPort();
};

function selectRole(role) {
    inputHostPort();
    document.getElementById("role-title").innerHTML = role.value;
    if(role.value === "*** Server ***"){
        document.getElementById("main").style.display = "none";
        document.getElementById("main2").style.width = "100%";
        document.getElementById("btn-run").style.display = "block";
        document.getElementById("btn-connect").style.display = "none";
    } else {
        document.getElementById("main").style.display = "block";
        document.getElementById("main2").style.width = "30%";
        document.getElementById("btn-run").style.display = "none";
        document.getElementById("btn-connect").style.display = "block";
    }
    
}

function logMes(mes){
    document.getElementById("logs").innerHTML += 
        `<div class="">${mes}</div>`; 
};

// module.exports.logMes = logMes;
/*
let clientCount = 0;
function addCLient(){
    ++clientCount;
    // for(let index = 0; index < 3; index++) { <h3>Client ${index+1}</h3>
        document.getElementById("clients_container").innerHTML += 
        `<div class="list-write-to__item">
            <h3>Client ${clientCount}</h3>
            <div class="mes d-flex jc-sb">
                <div class="mes-area ">
                    <div class="mes-viewport viewport"></div>
                    <div class="mes-act d-flex">
                        <input id="mes-mes" class="viewport" type="text">
                        <button onclick="sendMes()">Send</button>
                        <button id="btn-connect" onclick="clientConnect('Client ${clientCount}')">Connect</button>
                    </div>
                </div>
                <div class="write-to-cont-main d-flex flex-dir-col jc-sb">
                    <div class="write-to-cont d-flex flex-dir-col">
                        <h3>Write to:</h3>
                        <div class="list-write-to viewport"></div>
                    </div>
                </div>
            </div>
        </div>`; 
    // }
};
*/


