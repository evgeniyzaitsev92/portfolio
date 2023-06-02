const net = require("net");

let sockets = [];
let arrOccupiedIDs = [];
let IDsAndNames = {};
let IDsAndSockets = {};
function setID(name, socket){
    for(let i=1; i<100; i++){
        if(!arrOccupiedIDs.includes(i)){
            arrOccupiedIDs.push(i);
            IDsAndNames[i] = name;
            IDsAndSockets[i] = socket;
            return i;
        }
    } 
};

let server;
function listenServer(){
    // if(document.getElementById("btn-run").innerHTML === "Run"){
        // document.getElementById("btn-run").innerHTML = "Stop";
        document.getElementById("role").disabled = true;
        document.getElementById("host").disabled = true;
        document.getElementById("port").disabled = true;
        document.getElementById("btn-run").disabled = true;
        logMes("** >>> Server is launched <<< **");
        
        server = net.createServer();
        server.on("connection", (socket) => {
            socket.on("data", (data) => {
                const Data = JSON.parse(data.toString());

                // add client
                if(Data.type === "clientName"){
                    const id = setID(Data.value, socket);
                    socket.write(JSON.stringify({type: "setID", value: id})); 
                    sockets.push(socket);
                    setTimeout(() => {
                        for(let sock of sockets){
                            sock.write(JSON.stringify({type: "clientsNamesList", value: IDsAndNames}));
                        }
                    }, 500);
                    logMes(`-- Client '${Data.value}' has connected with '${id}' ID.`);

                // forward message
                } else if(Data.type === "sendMes"){
                    const recID = Data.recID;
                    const senderID = Data.senderID;
                    const sock = IDsAndSockets[recID];
                    sock.write(JSON.stringify({type: "recMes", mes: Data.mes, senderID: senderID}));
                }
                /*
                // remove
                else if(Data.type === "idRemove"){
                    console.log("12345");
                    // remove socket from sockets array
                    const indexSockets = sockets.indexOf(socket);
                    if (indexSockets !== -1) {
                        sockets.splice(indexSockets, 1);
                    }

                    // remove id from arrOccupiedIDs
                    const indexArrOccupiedIDs = arrOccupiedIDs.indexOf(Data.id);
                    if (indexArrOccupiedIDs !== -1) {
                        arrOccupiedIDs.splice(indexArrOccupiedIDs, 1);
                    }

                    // remove from IDsAndNames & IDsAndSockets
                    delete IDsAndNames[Data.id];
                    delete IDsAndSockets[Data.id];

                    logMes(`-- Client '${Data.name}' disconnected, '${Data.id}' ID is free.`);
                }
                */
            });

            socket.on("close", () => {
                // remove socket from sockets array
                const indexSockets = sockets.indexOf(socket);
                if (indexSockets !== -1) {
                    sockets.splice(indexSockets, 1);
                }

                // remove from IDsAndNames & IDsAndSockets
                let idRemove;
                let nameRemove;
                for (const item in IDsAndSockets) {
                    if(IDsAndSockets[item] == socket){
                        idRemove = item;
                        delete IDsAndSockets[idRemove];
                        nameRemove = IDsAndNames[idRemove];
                        delete IDsAndNames[idRemove];
                        break;
                    }
                }

                // remove id from arrOccupiedIDs
                const indexArrOccupiedIDs = arrOccupiedIDs.indexOf(Number(idRemove));
                if (indexArrOccupiedIDs !== -1) {
                    arrOccupiedIDs.splice(indexArrOccupiedIDs, 1);
                }

                for(let sock of sockets){
                    sock.write(JSON.stringify({type: "idRemove", value: Number(idRemove)}));
                }

                logMes(`-- Client '${nameRemove}' disconnected, '${Number(idRemove)}' ID is free.`);
            });

            socket.on("error", (err) => {
                logMes("-- "+err);
                console.log(err);
            });
        });
        server.listen(document.getElementById("port").value, document.getElementById("host").value);

    // } else {
    //     document.getElementById("btn-run").innerHTML = "Run";
    //     logMes("** >>> Server is stopped <<< **");
    //     server.close();
    // }
};
