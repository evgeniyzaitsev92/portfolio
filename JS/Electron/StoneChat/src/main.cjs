const {app, BrowserWindow} = require('electron');
const url = require('url');
const path = require('path'); 

let win;

function createWindow(){ 
   // win = new BrowserWindow({width: 1600, height: 900});
   win = new BrowserWindow({
      width: 1600, height: 900,
      webPreferences: {
          nodeIntegration: true,
          contextIsolation: false,
      }
  });
   
   win.loadURL(url.format ({ 
      pathname: path.join(__dirname, 'index.html'), 
      protocol: 'file:', 
      slashes: true 
   }));
   // win.webPreferences = {nodeIntegration: true, contextIsolation: true};
//    win.setMenuBarVisibility(false);
}  

app.on('ready', createWindow);