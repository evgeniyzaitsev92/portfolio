let socket = new net.Socket();
let currentName;
let id;
let idCurRec;
let chats = {};

// connect client to server
function clientConnect(name){   
    // if(document.getElementById("btn-connect").innerHTML === "Connect"){ 
    //     document.getElementById("btn-connect").innerHTML = "Disconnect";
        document.getElementById("role").disabled = true;
        document.getElementById("host").disabled = true;
        document.getElementById("port").disabled = true;
        document.getElementById("btn-connect").disabled = true;
        currentName = name;

        // connect to server
        socket.connect(document.getElementById("port").value, document.getElementById("host").value, () => {
            logMes("** >>> Connected <<< **");
            socket.write(JSON.stringify({type: "clientName", value: name})); 
        });

        // recieve data from server according to type
        socket.on("data", (data) => {
            const Data = JSON.parse(data.toString());

            // set client id
            if(Data.type === "setID"){
                id = Data.value;
                logMes(`-- Client '${name}' is assigned '${id}' ID.`);
            }

            // recieve list other clients names
            else if(Data.type === "clientsNamesList"){
                document.getElementById("list_write_to").innerHTML = ""; // clear recipient visual list
                let list = Data.value; // list of recipients
                delete list[id]; // delete current client from list

                // fill visual list
                for (const item in list) {
                    document.getElementById("list_write_to").innerHTML += 
                    `<div class="list-write-to__item">
                        <label id="${item}" class="d-flex jc-sb"><input type="radio" onchange="changeRecipient(this);" name="clients-list" value="${item}"><span>${list[item]}</span></label>
                    </div>`; 
                }

                // set current recipient
                setCurRec();

            // get message
            } else if(Data.type === "recMes"){
                if(!(Data.senderID in chats)){
                    chats[Data.senderID] = [];
                }
                chats[Data.senderID].push({rec: Data.mes});
                if(String(getRecID()) === String(Data.senderID)){
                    document.getElementById("mes_viewport").innerHTML += 
                        `<div class="mes-viewport__item-rec">${Data.mes.replace(/\n/g, '<br/>')}</div>`;
                }

            // remove from list
            } else if(Data.type === "idRemove"){
                let idRemove = Number(Data.value);
                if(idRemove in chats){
                    delete chats[idRemove];
                }
                const nameRemove = document.getElementById(`${idRemove}`).lastChild.innerHTML;
                const itemRemove = document.getElementById(`${idRemove}`);
                itemRemove.remove();

                // set current recipient
                setCurRec();

                logMes(`-- Client '${nameRemove}' has disconnected.`);
            }
        });

        /*
        socket.on("close", () => {
            logMes("-- Client disconnected.");
            socket.write(JSON.stringify({type: "idRemove", id: id, name: currentName}));
    
            return new Promise(resolve => {
                socket.once('close', () => resolve());
                socket.destroy();
            });
        });
        */
        
        socket.on("error", (err) => {
            logMes("-- "+err);
            console.log(err);
        });

    /*
    } else {
        document.getElementById("btn-connect").innerHTML = "Connect";
        logMes("-- Client disconnected.");
        socket.write(JSON.stringify({type: "idRemove", id: id, name: currentName}));
        id = undefined;

        return new Promise(resolve => {
            socket.once('close', () => resolve());
            socket.destroy();
        });
    }
    */
};

function setCurRec(){
    const recipientsList = document.getElementsByName("clients-list");

    // select recipient
    for(let i = 0; i < recipientsList.length; i++) {
        if(recipientsList[i].value === idCurRec){
            recipientsList[i].checked = true;
            break;
        } 
    }
}

function getRecID(){
    let recID;
    const recipientsList = document.getElementsByName("clients-list");
    // get selected recipient
    for(let i = 0; i < recipientsList.length; i++) {
        if(recipientsList[i].checked){
            recID = recipientsList[i].value;
            break;
        } 
    }
    return recID;
};

// send message
function changeRecipient(){
    document.getElementById("mes_viewport").innerHTML = "";
    idCurRec = getRecID();
    const arrChat = chats[idCurRec];

    if(arrChat == undefined || arrChat == null){
        return;
    }
    
    for (const item in arrChat){
        if(Object.keys(arrChat[item])[0] === "sent"){
            document.getElementById("mes_viewport").innerHTML += 
            `<div class="mes-viewport__item-sent">${arrChat[item].sent.replace(/\n/g, '<br/>')}</div>`;
        } else if(Object.keys(arrChat[item])[0] === "rec"){
            document.getElementById("mes_viewport").innerHTML += 
            `<div class="mes-viewport__item-rec">${arrChat[item].rec.replace(/\n/g, '<br/>')}</div>`;
        }
    }
};

// send message
function sendMes(){
    const recipientsList = document.getElementsByName("clients-list");
    const mes = document.getElementById("mes_mes").value;
    const recID = getRecID();

    // check if list is not empty and recipient is really selected
    if(recipientsList.length === 0 || recID == undefined || recID == null || mes === ""){
        return;
    }

    // add sent message
    if(!(recID in chats)){
        chats[recID] = [];
    }
    chats[recID].push({sent: mes});
    
    // console.log(chats);
    document.getElementById("mes_viewport").innerHTML += 
                `<div class="mes-viewport__item-sent">${mes.replace(/\n/g, '<br/>')}</div>`;
    document.getElementById("mes_mes").value = "";
    socket.write(JSON.stringify({type: "sendMes", mes: mes, recID: recID, senderID: id})); 
};