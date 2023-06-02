const win = nw.Window.get();
const fs = require("fs-extra");
const fsPromises = require("fs").promises;
const childProcess = require("child_process");
const ytdl = require("ytdl-core");
const splitFile = require("split-file");
const winattr = require("winattr");
const ffmpeg = require("fluent-ffmpeg");
const youtubeCaptionsScraper = require("youtube-captions-scraper");
const checkInternetConnected = require("check-internet-connected");
const path = require("path");

let num = 0;
let arrIDsList = [];

// const appDir = path.dirname(process.execPath); // FOR COMPILED
const appDir = nw.App.startPath; // FOR DEV
const downloads = `${appDir}\\downloads`;
const temp = `${appDir}\\downloads\\~temp`;
const tempHelpers = `${appDir}\\downloads\\~temp\\helpers`;
const tempRubbish = `${appDir}\\downloads\\~temp\\helpers\\rubbish`;
const dataJSON = `${appDir}\\downloads\\~temp\\data.json`;
// console.log(process.versions["node-webkit"]); // 0.72.0

const settingsPath = `${appDir}\\settings.json`;

// Set download path on start
if(fs.existsSync(settingsPath)){
    const settingsData = fs.readFileSync(settingsPath);
    if(settingsData.length > 0){
        const settings = JSON.parse(settingsData);
        if("downloadPath" in settings){
            document.getElementById("download_folder_path").value = settings["downloadPath"];
        }   
    }
}

/*** Check Internet connection ***/
function isInternetOnline(callback){
    checkInternetConnected().then((result) => {
        if(result === true){ callback(true); } else { callback(false);}
    }).catch((ex) => {
        callback(false);
        console.log(ex);
    });
};

function checkInternet(func, ...args){
    // let online = false;
    isInternetOnline(function(isOnline){
        if (!isOnline) {
            alert("Интернет-соединение отсутствует!");
        } else {
            func(...args);
        }
    });
};
/************************/

// Set size when start app or resize window
function setSize(){
    document.getElementById("download").style.width = `${win.width-25}px`;
    document.getElementById("download").style.height = `${win.height-203}px`;
};
setSize();

// Create default folders
function createFolders(){
    // default download folder
    if(!fs.existsSync(downloads)){
        fs.mkdirSync(downloads);
    }

    // MAIN folder for temporary files during downloading
    if(!fs.existsSync(temp)){
        fs.mkdirSync(temp);
        
    }

    // information about paused downloads
    if(!fs.existsSync(dataJSON)){
        fs.writeFileSync(dataJSON, "");
    }
    
    // folder for temporary files during downloading
    if(!fs.existsSync(tempHelpers)){
        fs.mkdirSync(tempHelpers);
    }

    // folder for temporary files that are to be removed
    if(!fs.existsSync(tempRubbish)){
        fs.mkdirSync(tempRubbish);
    }

    // temp folder should be hidden
    winattr.setSync(temp, {hidden: true});
};
createFolders();

// Processing on input fields
function processInputField(id, action){
    // clear field
    if(action === "clear"){
        document.getElementById(id).value = "";

    // copy link into url field from buffer
    } else if(action === "copy_link"){
        navigator.clipboard.readText().then((clipText) => {
            if(clipText.startsWith("http")){
                document.getElementById(id).value = clipText;
            }
        });
    }
};

// Set download folder
function setFolderPath(isDefault){
    // set folder browsering in file system
    if(isDefault === false){
        document.getElementById("download_folder_path").value = document.getElementById("download_folder").value;

    // set default download folder in the app root directory
    } else {
        document.getElementById("download_folder_path").value = downloads;
    }

    // save path to settings file
    const downloadPath = document.getElementById("download_folder_path").value;
    if(!fs.existsSync(settingsPath)){
        fs.writeFileSync(settingsPath, "");
        const downloadPathData = JSON.stringify({downloadPath: downloadPath});
        fs.writeFileSync(settingsPath, downloadPathData);
    } else {
        const settingsData = fs.readFileSync(settingsPath);
        if(settingsData.length > 0){
            const settings = JSON.parse(settingsData);
            settings["downloadPath"] = downloadPath;
            const settingsStr = JSON.stringify(settings);
            fs.writeFileSync(settingsPath, settingsStr);
        } else {
            const downloadPathData = JSON.stringify({downloadPath: downloadPath});
            fs.writeFileSync(settingsPath, downloadPathData);
        }
    } 
};

// get random number
function getRandomInt(max){
    return Math.floor(Math.random() * max);
};

function moveTempFilesToRubbish(downloadName, container, isMP3){
    const tempName = `${tempHelpers}\\${downloadName}.${container}`;
    if(fs.existsSync(tempName) && isMP3 === false){
        const newTempName = `${tempRubbish}\\${downloadName}${getRandomInt(1000000000)}.${container}`;
        fs.renameSync(tempName, newTempName);
        fs.writeFileSync(newTempName, "", function(){console.log("done")});
    }
    if(fs.existsSync(`${tempName}.temp1`)){
        const newTempName = `${tempRubbish}\\${downloadName}${getRandomInt(1000000000)}.${container}.temp1`;
        fs.renameSync(`${tempName}.temp1`, newTempName);
        fs.writeFileSync(newTempName, "", function(){console.log("done")});
    }
    if(fs.existsSync(`${tempName}.temp2`)){
        const newTempName = `${tempRubbish}\\${downloadName}${getRandomInt(1000000000)}.${container}.temp2`;
        fs.renameSync(`${tempName}.temp2`, newTempName);
        fs.writeFileSync(newTempName, "", function(){console.log("done")});
    }
};

// Check whether link is valid or not
function checkValidVideo(ID) {
    var img = new Image();
    img.src = "http://img.youtube.com/vi/" + ID + "/mqdefault.jpg";
    img.onload = function () {
        // checkThumbnail(this.width);
        if (this.width === 120) {
            alert("Невалидная ссылка на видео!");
            return false;
        }
    }
    return true;
};

// Check url in the input field
function checkURL(){
    let ID;
    let url = document.getElementById("url").value;
    if(url === ""){
        alert("Введите ссылку на видео!");
        return "";
    }

    /*
    if(!url.startsWith("https://www.youtube.com/watch?v=") && !url.startsWith("http://www.youtube.com/watch?v=") 
            && !url.startsWith("http://youtu.be/")){
        alert("Невалидная ссылка на видео!");
    }
    */

    if(url.includes("youtu.be")){
        ID = url.split("youtu.be/")[1].substring(0, 11);
    } else if(url.includes("v=")){
        ID = url.split("v=")[1].substring(0, 11);
    } else if(url.includes("shorts")){
        ID = url.split("shorts/")[1].substring(0, 11);
    }

    if(!checkValidVideo(ID)){
        alert("Невалидная ссылка на видео!");
        return "";
    }

    url = `https://www.youtube.com/watch?v=${ID}`; // construct normal link after confirming that url with such ID exists
    document.getElementById("url").value = url;
    document.getElementById(`btn_download`).dataset.url = url;
    document.getElementById(`btn_download_mp3`).dataset.url = url;
    document.getElementById(`btn_download_subt`).dataset.url = url;
    return url;
};

// Analyze video to get info about it
async function analyze(){
    document.getElementById("btn_anlz_process_cont").style.display = "block";
    const url = checkURL(); // check url on validity
    if(url === ""){
        document.getElementById("btn_anlz_process_cont").style.display = "none";
        return;
    }

    let info = await ytdl.getInfo(url); // get all info
    // console.log(info);
    let videoDetails = info["videoDetails"];
    document.getElementById("download_name").value = videoDetails["title"]; // get title and put it in the name input field
    // document.getElementById(`tooltip_btn_download_title`).innerHTML = videoDetails["title"];
    // const ID = url.split("v=")[1].substring(0, 11);
    // document.getElementById(`tooltip_btn_download_img`).src = `http://img.youtube.com/vi\\${ID}/mqdefault.jpg`;

    // shortern number
    function formatCompactNumber(number){
        if (number < 1000) {
          return number;
        } else if (number >= 1000 && number < 1_000_000) {
          return (number / 1000).toFixed(1) + " тыс.";
        } else if (number >= 1_000_000 && number < 1_000_000_000) {
          return (number / 1_000_000).toFixed(1) + " млн";
        } else if (number >= 1_000_000_000 && number < 1_000_000_000_000) {
          return (number / 1_000_000_000).toFixed(1) + " млрд";
        } else if (number >= 1_000_000_000_000 && number < 1_000_000_000_000_000) {
          return (number / 1_000_000_000_000).toFixed(1) + " трлн";
        }
      }

    // video info
    document.getElementById("video_info_title").innerHTML = videoDetails["title"];
    document.getElementById("video_info_channel").innerHTML = videoDetails["author"]["name"];
    document.getElementById("video_info_published").innerHTML = videoDetails["publishDate"];
    document.getElementById("video_info_length").innerHTML = new Date(Number(videoDetails["lengthSeconds"]) * 1000).toISOString().slice(11, 19);
    document.getElementById("video_info_view_count").innerHTML = formatCompactNumber(Number(videoDetails["viewCount"]));
    document.getElementById("video_info_desc").innerHTML = videoDetails["description"];

    /*** Availale formats ***/ 
    let list = info["formats"]; // array list
    let listVideoAudio = [];
    let listVideoNoAudio = [];
    let listAudio = [];

    // fill two arrays - with video and audio separately
    for (const item of list) {
        if (item["hasVideo"] === true) {
            if (item["hasAudio"] === true) {
                listVideoAudio.push(item);
            } else {
                listVideoNoAudio.push(item);
            }
        } else {
            listAudio.push(item);
        }
    }

    // sort by video height
    listVideoAudio.sort((a, b) => {
        return b["height"] - a["height"];
    });
    listVideoNoAudio.sort((a, b) => {
        return b["height"] - a["height"];
    });

    // sort by bitrate that has audio only
    listAudio.sort((a, b) => {
        return b["audioBitrate"] - a["audioBitrate"];
    });

    list = [];
    list.push(...listVideoAudio);
    list.push(...listVideoNoAudio);
    list.push(...listAudio);
    // console.log(list);

    // fill select list of available formats
    let num = 0;
    document.getElementById("select_format").innerHTML = "";
    for(const item of list){
        num++;
        const qualityLabel = (item["qualityLabel"] == null ? "" : item["qualityLabel"] + ", ");
        const hasVideo = (item["hasVideo"] === true ? "да" : "нет");
        const hasAudio = (item["hasAudio"] === true ? `да, ${item["audioBitrate"]} кбит/с, ${item["audioSampleRate"]} Гц` : "нет");
        const selected = (num === 1 ? "selected" : "");
        const size = (item["contentLength"] == undefined ? "???" : `${(Number(item["contentLength"])/1048576).toFixed(2)}`);
        document.getElementById("select_format").innerHTML += 
            `<option value="${item["itag"]}" data-quality="${item["qualityLabel"]}" data-audio="${item["hasAudio"]}" data-bitrate="${item["audioBitrate"]}" data-container="${item["container"]}" data-size="${item["contentLength"]}" ${selected}>
                ${qualityLabel}видео: ${hasVideo}, аудио: ${hasAudio}, формат: ${item["container"]}, ${size} МБ
            </option>`;
    }
    selectFormatChange();
    /************************/

    // subtitles
    if("captions" in info["player_response"]){
        document.getElementById("btn_download_subt").classList.remove("btn-disabled");
        document.getElementById("btn_download_subt").classList.add("btn-green");
        document.getElementById("btn_download_subt").classList.add("frame-green");
        document.getElementById("btn_download_subt").disabled = false;
        let subtList = info["player_response"]["captions"]["playerCaptionsTracklistRenderer"]["captionTracks"];
        document.getElementById("select_subtitles").innerHTML = "";
        for(const item of subtList){
            document.getElementById("select_subtitles").innerHTML += 
                `<option value="${item["languageCode"]}">${item["name"]["simpleText"]}</option>`;
        }
    } else {
        document.getElementById("btn_download_subt").classList.add("btn-disabled");
        document.getElementById("btn_download_subt").classList.remove("btn-green");
        document.getElementById("btn_download_subt").classList.remove("frame-green");
        document.getElementById("btn_download_subt").disabled  = true;
    } 

    // set video in iframe
    let embed = url.replace("watch?v=", "embed/");
    document.getElementById("download_frame").src = embed;
    document.getElementById("btn_anlz_process_cont").style.display = "none";
};

// Clear download list
function clearList() {
    const progressConts = document.querySelectorAll(".progress-cont");
    for (const item of progressConts) {
        const num = item.dataset.num;
        const downloadName = document.getElementById(`progress_name${num}`).dataset.downloadName;
        const container = document.getElementById(`progress_name${num}`).dataset.container;
        moveTempFilesToRubbish(downloadName, container, false);
    }
    document.getElementById("download_list").innerHTML = "";
    fs.writeFileSync(dataJSON, "");
};

// Remove item from download list
function closeDownloaded(num){
    
    // remove temp files (helpers)
    const downloadName = document.getElementById(`progress_name${num}`).dataset.downloadName; 
    const container = document.getElementById(`progress_name${num}`).dataset.container;
    moveTempFilesToRubbish(downloadName, container, false);

    document.getElementById(`progress_cont${num}`).remove();
    document.getElementById(`tooltip_progress_path${num}`).remove();
    
    // remove from data.json
    let data = fs.readFileSync(dataJSON);
    const arrDownloads = JSON.parse(data);
    for(const item of arrDownloads){
        if(Number(item["num"]) === num){
            const index = arrDownloads.indexOf(item);
            if (index > -1) {
                arrDownloads.splice(index, 1);
                break;
            }
        }
    }
    data = JSON.stringify(arrDownloads);
    fs.writeFileSync(dataJSON, data);    
};

// Tooltip with path when clicking on downloads item`s name
function tooltipOnOff(num){
    if(document.getElementById(`progress_name${num}`).dataset.tooltip === "OFF"){
        document.getElementById(`progress_name${num}`).dataset.tooltip = "ON";
        document.getElementById(`tooltip_progress_path${num}`).style.display = "block";
    } else {
        document.getElementById(`progress_name${num}`).dataset.tooltip = "OFF";
        document.getElementById(`tooltip_progress_path${num}`).style.display = "none";
    }
};

function selectFormatChange(){
    const selectFormat = document.getElementById("select_format");
    if(selectFormat.options[selectFormat.selectedIndex].dataset.audio === "true"){
        document.getElementById("btn_download_mp3").classList.remove("btn-disabled");
        document.getElementById("btn_download_mp3").classList.add("btn-green");
        document.getElementById("btn_download_mp3").classList.add("frame-green");
        document.getElementById("btn_download_mp3").disabled = false;
    } else {
        document.getElementById("btn_download_mp3").classList.add("btn-disabled");
        document.getElementById("btn_download_mp3").classList.remove("btn-green");
        document.getElementById("btn_download_mp3").classList.remove("frame-green");
        document.getElementById("btn_download_mp3").disabled  = true;
    } 
};

// Downloads item progress container
function progressCont(num, url, folderPath, downloadName, container, itag, total, isFinished, onLoad, isMP3, bitrate, 
        isFinishedMP3, isCompleteMP3, isSubtitles, isSubtitlesFinished){
    let statusNameSigh;
    let statusTypeSigh;
    let statusPause;
    let statusPauseSigh;
    let statusFinished;
    let statusNameClass;
    let statusTypeClass;
    let statusTotal;
    let statusSubtHidden = "";
    let containerForName;

    if(isSubtitles === "false"){
        if(isMP3 === "true"){
            containerForName = "mp3";
            statusTypeClass = "bcg-frame-grey";
            statusTypeSigh = "./img/audio.png";
        } else {
            statusTypeClass = "bcg-frame-yellow";
            statusTypeSigh = "./img/video.png";
            containerForName = container;
        }

        if(onLoad){
            statusNameSigh = "./img/pause.png";
            statusPause = "ON";
            statusPauseSigh = "./img/play.png";
            statusTotal = total;
        } else {
            statusNameSigh = "./img/play.png";
            statusPause = "OFF";
            statusPauseSigh = "./img/pause.png";
            statusTotal = "";
        }
    
        if(isFinished){
            statusPause = "OFF";
            statusNameClass = "downloaded";
            statusFinished = "true";
            statusNameSigh = "";
            statusPauseSigh = "./img/pause.png";
        } else {
            statusFinished = "false";
            if(isMP3 === "true"){
                if(isFinishedMP3 === "true"){
                    statusPause = "OFF";
                    statusNameSigh = "";
                    statusPauseSigh = "./img/pause.png";
                    statusNameClass = "downloaded-mp3";
                } else {
                    statusNameClass = "bcg-frame-grey";
                }
            } else {
                statusNameClass = "bcg-frame-yellow";
            }
        }
    } else {
        statusTypeClass = "bcg-frame-dark-grey";
        statusTypeSigh = "./img/subtitles.png";
        statusSubtHidden = "downloaded-subtitles-hidden";
        if(isSubtitlesFinished === "false"){
            statusNameClass = "bcg-frame-dark-grey";
        } else {
            statusNameClass = "downloaded-subtitles";
        }
        statusNameSigh = "";
        statusPause = "";
        statusPauseSigh = "./img/pause.png";
        statusFinished = "";
        statusTotal = "";
    }

    return `
    <div class="progress-cont d-flex ai-cntr" id="progress_cont${num}" data-num="${num}">
        <button class="progress-btn btn bcg-frame-red" id="progress_btn_close${num}" onclick="closeDownloaded(${num})"><img src="./img/close.png" alt=""></button>
        <div class="progress-type ${statusTypeClass} d-flex ai-cntr" id="progress_type${num}">           
            <img id="progress_type_sign${num}" src="${statusTypeSigh}" alt="">
        </div>
        <div class="progress-name ${statusNameClass} d-flex ai-cntr has-tooltip" id="progress_name${num}" onclick="tooltipOnOff(${num})" data-tooltip="OFF" data-finished="${statusFinished}" data-url="${url}" data-folder-path="${folderPath}" data-download-name="${downloadName}" data-container="${container}" data-itag="${itag}" data-mp3="${isMP3}" data-bitrate="${bitrate}" data-finishedmp3="${isFinishedMP3}" data-completemp3="${isCompleteMP3}" data-subtitles="${isSubtitles}">           
            <img id="progress_name_sign${num}" src="${statusNameSigh}" alt=""><span id="progress_name_span${num}">${downloadName}.${containerForName}</span>
        </div>
        <div class="progress frame-green bcg-wht" id="progress${num}">
            <div class="progress-bar d-flex ai-cntr" id="progress_bar${num}"><span id="progress_bar_num${num}">0%</span></div>
        </div>
        <button class="${statusSubtHidden} progress-btn btn bcg-frame-red-sand" id="progress_btn_pause${num}" data-total="${statusTotal}" onclick="checkInternet(pauseDownload, ${num}, false)" value=${statusPause}><img src=${statusPauseSigh} alt=""></button>
        <button class="${statusSubtHidden} progress-btn btn bcg-frame-red-sand" id="progress_btn_stop_repeat${num}" onclick="checkInternet(stopRepeatDownload, ${num})" value="OFF"><img src="./img/stop.png" alt=""></button>
    </div>
    <div class="tooltip frame-brown bcg-wht d-flex flex-dir-col" id="tooltip_progress_path${num}">
        &#x21e7&#x21e7 ${folderPath}\\${downloadName}.${containerForName}
    </div> 
    `;
};

// Check if file is in download list
function checkPathForDownload(path, fromListOnly){
    let foundInList = false;
    if(document.getElementById("download_list").innerHTML !== ""){
        const progressConts = document.querySelectorAll(".progress-cont");
        for(const item of progressConts){
            const num = item.dataset.num;
            const folderPath = document.getElementById("download_folder_path").value;
            const name = document.getElementById(`progress_name_span${num}`).innerHTML;
            itemPath = `${folderPath}\\${name}`;
            if(itemPath === path){
                if(fromListOnly === false){
                    foundInList = true;
                    break;
                } else {
                    document.getElementById(`progress_cont${num}`).remove();
                    document.getElementById(`tooltip_progress_path${num}`).remove();
                    break;
                }
            }
        }
    }
    return foundInList;
};

// Prepare for downloading
function preDownload(url, isMP3, isSubtitles, newName){
    if(url === ""){
        alert("Введите ссылку на видео и проанализируйте его!");
        return;
    }

    if(isMP3 === "true" && !fs.existsSync(`${appDir}\\ffmpeg.exe`)){
        alert("Файл 'ffmpeg.exe' отсутствует в корневой папке!");
        return;
    }

    let downloadName = document.getElementById("download_name").value;
    if(newName !== ""){
        downloadName = newName;
    }
    const folderPath = document.getElementById("download_folder_path").value;
    const selectFormat = document.getElementById("select_format");
    const container = selectFormat.options[selectFormat.selectedIndex].dataset.container;
    const quality = selectFormat.options[selectFormat.selectedIndex].dataset.quality;
    const bitrate = Number(selectFormat.options[selectFormat.selectedIndex].dataset.bitrate);
    const itag = document.getElementById("select_format").value;
    const subtitles = document.getElementById("select_subtitles");
    let subtitlesText = "";
    let subtitlesCode = "";
    if(subtitles.innerHTML !== ""){
        subtitlesText = subtitles.options[subtitles.selectedIndex].text;
        subtitlesCode = document.getElementById("select_subtitles").value;
    }
    
    // download name not defined
    if(downloadName === ""){
        alert("Введите имя файла!");
        return;
    }

    // download path not defined
    if(folderPath === ""){
        alert("Укажите путь сохранения файла!");
        return;
    }

    // download path does not exist
    if(!fs.existsSync(`${folderPath}`)){
        alert("Путь не существует!");
        return;
    }

    /*** Prevent certain symbols in name ***/ 
    downloadName = downloadName.replace(":", "~");
    downloadName = downloadName.replace("?", "~");
    downloadName = downloadName.replace("*", "~");
    downloadName = downloadName.replace("<", "~");
    downloadName = downloadName.replace(">", "~");
    downloadName = downloadName.replace("/", "~");
    downloadName = downloadName.replace("|", "~");
    downloadName = downloadName.replace("\\", "~");
    downloadName = downloadName.replace("\"", "~");
    /************************/

    if(isSubtitles === "false"){
        let cont;
        if(isMP3 === "false"){
            if(quality !== "null"){
                downloadName += `_${quality}`; // add quality to name as postfix
            }
            cont = container;
        } else {
            downloadName += `_${container.toUpperCase()}`;
            if(bitrate !== "null"){
                downloadName += `_${bitrate}kbps`; // add bitrate to name as postfix
            }
            cont = "mp3";
        }

        // check if output file already exists
        if(fs.existsSync(`${folderPath}\\${downloadName}.${cont}`)){
            const doReplace = prompt("Файл с таким именем существует. Введите новое имя или оставьте текущее, заменив существующий файл.", downloadName);
            if(doReplace == null || doReplace == ""){
                return;
            } else {
                downloadName = doReplace;
                if(fs.existsSync(`${folderPath}\\${downloadName}.${cont}`)){
                    preDownload(url, isMP3, isSubtitles, downloadName);
                    return;
                }
            }
        }

        // check if file is being downloaded already
        if(fs.existsSync(`${tempHelpers}\\${downloadName}.${container}`) && checkPathForDownload(`${folderPath}\\${downloadName}.${cont}`, false) === true) {
            alert(`Скачивание файла "${folderPath}\\${downloadName}.${cont}" уже идёт!`); 
            return;
        } else if(!fs.existsSync(`${tempHelpers}\\${downloadName}.${container}`) && checkPathForDownload(`${folderPath}\\${downloadName}.${cont}`, false) === true) {
            checkPathForDownload(`${folderPath}\\${downloadName}.${container}`, true); // if only in download list, remove it
        }

    } else {
        downloadName += `_${subtitlesCode.toUpperCase()}`; // add subtitles code to name as postfix
        if(subtitlesText.includes("auto-generated")){
            downloadName += `_auto-generated`; // add "auto-generated" to name as postfix
        }
        if(fs.existsSync(`${folderPath}\\${downloadName}.txt`)){
            const doReplace = prompt("Файл с таким именем существует. Введите новое имя или оставьте текущее, заменив существующий файл.", downloadName);
            if(doReplace == null || doReplace == "") {
                return;
            } else {
                downloadName = doReplace;
                if(fs.existsSync(`${folderPath}\\${downloadName}.${cont}`)){
                    preDownload(url, isMP3, isSubtitles, downloadName);
                    return;
                }
            }
        }
    }  
    
    ++num;

    // add progress container to downloads list and start downloading
    if(isSubtitles === "false"){
        // video
        if(isMP3 === "false"){
            document.getElementById("download_list").innerHTML += progressCont(num, url, folderPath, downloadName, container, itag, 0, false, false, isMP3, bitrate, "false", "false", isSubtitles, "false");
            download(num, url, folderPath, downloadName, container, itag, false, 0, isMP3, bitrate);

        // mp3
        } else {
            if(selectFormat.options[selectFormat.selectedIndex].dataset.audio === "true"){
                document.getElementById("download_list").innerHTML += progressCont(num, url, folderPath, downloadName, container, itag, 0, false, false, isMP3, bitrate, "false", "false", isSubtitles, "false");
                download(num, url, folderPath, downloadName, container, itag, false, 0, isMP3, bitrate);
            } else {
                alert("Не содержит аудио!");
            }   
        }

    // subtitles
    } else {
        document.getElementById("download_list").innerHTML += progressCont(num, url, folderPath, downloadName, "txt", 0, 0, false, false, isMP3, 0, "false", "false", isSubtitles, "false");
        downloadSubtitles(num, url, folderPath, downloadName, subtitlesCode);
    }
};

// Download sutitles
function downloadSubtitles(num, url, folderPath, downloadName, subtitlesCode){
    const ID = url.split("v=")[1].substring(0, 11);
    const subtitlesPath = `${folderPath}\\${downloadName}.txt`;
    
    youtubeCaptionsScraper.getSubtitles({
        videoID: ID, // youtube video id
        lang: subtitlesCode // default: `en`
    }).then(function(subtitles){
        if(!fs.existsSync(subtitlesPath) || fs.existsSync(subtitlesPath)){
            fs.writeFileSync(subtitlesPath, "", function(){console.log("done")});
        }

        // write sutitles to file
        let numSub = 0;
        for(const item of subtitles){
            numSub++;
            const startNum = Number(item["start"]);
            const durNum = Number(item["dur"]);
            const endNum = startNum+durNum;
            const start = new Date(startNum * 1000).toISOString().slice(11, 19);
            const end = new Date(endNum * 1000).toISOString().slice(11, 19);
            fs.appendFileSync(subtitlesPath, `>>> *** ${numSub} *** <<<\n${start} - ${end}\n${item["text"]}\n\n`);
            const percent = (numSub / subtitles.length) * 100;
            document.getElementById(`progress_bar${num}`).style.width = percent + "%";
            document.getElementById(`progress_bar_num${num}`).innerHTML = 
                `${Math.round(percent)}%`;            
        }

        document.getElementById(`progress_name${num}`).classList.remove("bcg-frame-dark-grey");
        document.getElementById(`progress_name${num}`).classList.add("downloaded-subtitles");

        notifyFinished();
    });
};

// Stop/repeat downloading
function stopRepeatDownload(num){
    // prevent if finished mp3
    if (document.getElementById(`progress_name${num}`).dataset.finishedmp3 === "true"){
        // return;
    }

    const url = document.getElementById(`progress_name${num}`).dataset.url; 
    const folderPath = document.getElementById(`progress_name${num}`).dataset.folderPath; 
    const downloadName = document.getElementById(`progress_name${num}`).dataset.downloadName; 
    const container = document.getElementById(`progress_name${num}`).dataset.container; 
    const itag = document.getElementById(`progress_name${num}`).dataset.itag;
    const isMP3 = document.getElementById(`progress_name${num}`).dataset.mp3;
    const bitrate = document.getElementById(`progress_name${num}`).dataset.bitrate;

    // stop
    if (document.getElementById(`progress_btn_stop_repeat${num}`).value === "OFF"){
        // remove temp files (helpers)
        moveTempFilesToRubbish(downloadName, container, false);

        // check Internet connection
        if (document.getElementById(`progress_btn_pause${num}`).value === "ON"){
            pauseDownload(num, true);
        }

        document.getElementById(`progress_name_sign${num}`).src = "./img/stop.png";
        document.getElementById(`progress_btn_stop_repeat${num}`).value = "ON";
        document.getElementById(`progress_btn_stop_repeat${num}`).innerHTML = `<img src="./img/repeat.png" alt="">`;

    // repeat
    } else {
        document.getElementById(`progress_name_sign${num}`).src = "./img/play.png";
        document.getElementById(`progress_btn_stop_repeat${num}`).value = "OFF";
        document.getElementById(`progress_btn_stop_repeat${num}`).innerHTML = `<img src="./img/stop.png" alt="">`;

        if (document.getElementById(`progress_name${num}`).dataset.finished === "true" && isMP3 === "false"){
            document.getElementById(`progress_name${num}`).classList.remove("downloaded");
            document.getElementById(`progress_name${num}`).classList.add("bcg-frame-yellow");
        }
        if (isMP3 === "true"){
            document.getElementById(`progress_name${num}`).dataset.finishedmp3 = "false";   
            document.getElementById(`progress_name${num}`).dataset.completemp3 = "false";   
            document.getElementById(`progress_name${num}`).classList.remove("downloaded-mp3");
            document.getElementById(`progress_name${num}`).classList.remove("downloaded");
            document.getElementById(`progress_name${num}`).classList.add("bcg-frame-grey");
        }
        
        document.getElementById(`progress_name${num}`).dataset.finished = "false";     
        download(num, url, folderPath, downloadName, container, itag, false, 0, isMP3, bitrate);
    }
};

// Pause/proceed downloading
function pauseDownload(num, isStopped){
    // prevent if stopped or finished
    if (document.getElementById(`progress_btn_stop_repeat${num}`).value === "ON" || document.getElementById(`progress_name${num}`).dataset.finished === "true" || document.getElementById(`progress_name${num}`).dataset.finishedmp3 === "true"){
        return;
    }

    // pause
    if (document.getElementById(`progress_btn_pause${num}`).value === "OFF") {
        document.getElementById(`progress_name_sign${num}`).src = "./img/pause.png";
        document.getElementById(`progress_btn_pause${num}`).value = "ON";
        document.getElementById(`progress_btn_pause${num}`).innerHTML = `<img src="./img/play.png" alt="">`;

    // proceed
    } else {
        document.getElementById(`progress_name_sign${num}`).src = "./img/play.png";
        document.getElementById(`progress_btn_pause${num}`).value = "OFF";
        document.getElementById(`progress_btn_pause${num}`).innerHTML = `<img src="./img/pause.png" alt="">`;
        const url = document.getElementById(`progress_name${num}`).dataset.url; 
        const folderPath = document.getElementById(`progress_name${num}`).dataset.folderPath; 
        const downloadName = document.getElementById(`progress_name${num}`).dataset.downloadName; 
        const container = document.getElementById(`progress_name${num}`).dataset.container; 
        const itag = document.getElementById(`progress_name${num}`).dataset.itag;
        const end = Number(document.getElementById(`progress_btn_pause${num}`).dataset.total);
        const isMP3 = document.getElementById(`progress_name${num}`).dataset.mp3;
        const bitrate = Number(document.getElementById(`progress_name${num}`).dataset.bitrate);
        if(!isStopped){
            download(num, url, folderPath, downloadName, container, itag, true, end, isMP3, bitrate);
        }
    }
};

// DOWNLOAD video or mp3
async function download(num, url, folderPath, downloadName, container, itag, isPaused, end, isMP3, bitrate){
    createFolders();
    let startTime;
    let stream;
    let outputFile;
    let start;
    // let tempMP3 = (isMP3 === "true") ? tempHelpers : folderPath;

    // if there were no pauses after start
    if(isPaused === false){
        // START downloading
        stream = ytdl(url, { quality: itag });
        outputFile = `${tempHelpers}\\${downloadName}.${container}`;

    // if proceed after pause
    } else {
        // if temp files (helpers) were removed
        if(!fs.existsSync(`${tempHelpers}\\${downloadName}.${container}`)){
            alert(`Скачивание "${folderPath}\\${downloadName}.${container}" невозможно! Обновите загрузку.`);
            stopRepeatDownload(num);
            return;
        }

        fs.copyFileSync(`${tempHelpers}\\${downloadName}.${container}`, `${tempHelpers}\\${downloadName}.${container}.temp1`); // copy downloaded as temp file (helper)
        let stat = fs.statSync(`${tempHelpers}\\${downloadName}.${container}`);
        start = stat.size; // get size of downloaded to proceed downloading from the right point

        // START downloading
        stream = ytdl(url, { quality: itag, range: {start: start, end: end} });
        outputFile = `${tempHelpers}\\${downloadName}.${container}.temp2`;
    }

    // merge temp files (helpers)
    function merge(folderPath, downloadName, container, isDownloaded, isMP3){
        const names = [`${tempHelpers}\\${downloadName}.${container}.temp1`, `${tempHelpers}\\${downloadName}.${container}.temp2`];
        splitFile.mergeFiles(names, `${tempHelpers}\\${downloadName}.${container}`).then(() => {
            if(isDownloaded){
                if(isMP3 === "false"){
                    fs.copyFileSync(`${tempHelpers}\\${downloadName}.${container}`, `${folderPath}\\${downloadName}.${container}`);
                }
                moveTempFilesToRubbish(downloadName, container, isMP3);
            }
        }).catch((err) => {
            console.log("Error: ", err);
        });
        
    }
    
    // main
    stream.on("progress", (_, downloaded, total) => {
        if(document.getElementById(`progress_btn_pause${num}`).dataset.total === ""){
            document.getElementById(`progress_btn_pause${num}`).dataset.total = total;
        }
        
        // if pause
        if(document.getElementById(`progress_btn_pause${num}`).value === "ON"){
            // if there were no pauses after start
            if(isPaused === false){
                document.getElementById(`progress_btn_pause${num}`).dataset.total = total;

            // if proceed after pause
            } else {
                if((start+downloaded) !== end){
                    merge(folderPath, downloadName, container, false, isMP3);
                    document.getElementById(`progress_btn_pause${num}`).dataset.total = end;
                }
            }
            stream.destroy(); // destroy stream
        }
        
        // if stop
        if(document.getElementById(`progress_btn_stop_repeat${num}`).value === "ON"){
            stream.destroy(); // destroy stream
        }

        let percent;
        let downloadedMinutes;
        let estimatedDownloadTime;
        let downloadedFin;
        let totalFin;

        /*** Change progress ***/
        // if there were no pauses after start
        if(isPaused === false){
            downloadedFin = downloaded; 
            totalFin = total;

        // if proceed after pause
        } else {
            downloadedFin = start + downloaded; 
            totalFin = end;
        }

        percent = downloadedFin / totalFin;
        downloadedMinutes = (Date.now() - startTime) / 1000;
        estimatedDownloadTime = (downloadedMinutes / (downloaded/total)) - downloadedMinutes;
        document.getElementById(`progress_bar${num}`).style.width = percent * 100 + "%";
        document.getElementById(`progress_bar_num${num}`).innerHTML = 
            `${Math.round(percent * 100)}% - (${(downloadedFin/1048576).toFixed(2)}/${(totalFin/1048576).toFixed(2)} МБ) - 
            ${new Date(estimatedDownloadTime * 1000).toISOString().slice(11, 19)}`;
        /************************/
        
    // on start
    }).once("response", (_, downloaded, total) => {
        startTime = Date.now();

    // on end, finished
    }).on("end", (_, downloaded, total) => {
        // if proceed after pause
        if(isPaused === true){
            merge(folderPath, downloadName, container, true, isMP3);
        } else {
            if(isMP3 === "false"){
                fs.renameSync(`${tempHelpers}\\${downloadName}.${container}`, `${folderPath}\\${downloadName}.${container}`);
            }
        }
        document.getElementById(`progress_name${num}`).dataset.finished = "true";
        document.getElementById(`progress_name${num}`).classList.remove("bcg-frame-yellow");
        document.getElementById(`progress_name${num}`).classList.add("downloaded");
        document.getElementById(`progress_name_sign${num}`).src = "";

        // if downloading mp3
        if(isMP3 === "true"){
            document.getElementById(`progress_name${num}`).dataset.finished = "false";
            document.getElementById(`progress_name${num}`).dataset.finishedmp3 = "true";
            document.getElementById(`progress_name${num}`).classList.remove("bcg-frame-grey");
            document.getElementById(`progress_name${num}`).classList.add("downloaded-mp3");
            document.getElementById(`progress_bar${num}`).innerHTML = 
            `<img id="" class="mp3-conv-sign mp3-conv-sign-rot" src="./img/conversion.png" alt=""><span id="progress_bar_num${num}">--> mp3</span>`;
            ffmpeg(`${tempHelpers}\\${downloadName}.${container}`)
            .audioBitrate(bitrate)
            .save(`${folderPath}\\${downloadName}.mp3`)
            .on("progress", p => {
            })
            .on("end", () => {
                // document.getElementById(`progress_name${num}`).dataset.finishedmp3 = "false";
                document.getElementById(`progress_name${num}`).dataset.completemp3 = "true";
                if(fs.existsSync(`${tempHelpers}\\${downloadName}.${container}`)){
                    moveTempFilesToRubbish(downloadName, container, false);
                }
                document.getElementById(`progress_bar${num}`).innerHTML = `<span id="progress_bar_num${num}">100%</span>`;
                notifyFinished();
            })
        } else {
            notifyFinished();
        }
    }).pipe(fs.createWriteStream(outputFile));   
};

function notifyFinished(){
    const audio = new Audio("../audio/notification3.mp3");
    audio.volume = 0.5;
    audio.play();
};

function pauseStartDowloads(onOff){
    const progressConts = document.querySelectorAll(".progress-cont");
    for (const item of progressConts) {
        const num = item.dataset.num;
        // pause download if in process
        if(document.getElementById(`progress_btn_pause${num}`).value === onOff) {
            pauseDownload(num, false);
        }
    }
};

/*** Window events ***/
win.on("resize", function (){
    setSize();
});

win.on("maximize", function (){
    setSize();
});

win.on("restore", function (){
    setSize();
});

// Fill downloads list retrieving info from data.json on start
win.on("loaded", function () {
    const data = fs.readFileSync(dataJSON);
    if(data.length === 0){
        return;
    }
    const arrDownloads = JSON.parse(data);
    for(const item of arrDownloads){
        const NUM = Number(item["num"]);
        const url = item["url"];
        const folderPath = item["folderPath"];
        const downloadName = item["downloadName"];
        const container = item["container"];
        const itag = item["itag"];
        const isMP3 = item["mp3"];
        const isFinishedMP3 = item["finishedmp3"];
        const isCompleteMP3 = item["completeMP3"];
        const bitrate = Number(item["bitrate"]);
        const total = Number(item["total"]);
        const isSubtitles = item["subtitles"];
        num = NUM;
        let downloaded = "";

        if(fs.existsSync(`${tempHelpers}\\${downloadName}.${container}`)){
            let stat = fs.statSync(`${tempHelpers}\\${downloadName}.${container}`);
            downloaded = stat.size;
        }
        if(fs.existsSync(`${folderPath}\\${downloadName}.${container}`)){
            let stat = fs.statSync(`${folderPath}\\${downloadName}.${container}`);
            downloaded = stat.size;
        }
        
        let startTime = Date.now();
        let percent = downloaded / total;
        let downloadedMinutes = (Date.now() - startTime) / 1000 / 60;
        let estimatedDownloadTime = (downloadedMinutes / (downloaded/total)) - downloadedMinutes;
        const finished = (percent === 1) ? true : false;

        document.getElementById("download_list").innerHTML += progressCont(num, url, folderPath, downloadName, container, itag, total, finished, true, isMP3, bitrate, isFinishedMP3, isCompleteMP3, isSubtitles, "true");
        if(isSubtitles === "false"){
            // video or audio (not finished)
            if(isFinishedMP3 === "false"){
                document.getElementById(`progress_bar${num}`).style.width = percent * 100 + "%";
                document.getElementById(`progress_bar_num${num}`).innerHTML = 
                `${Math.round(percent * 100)}% - (${(downloaded/1048576).toFixed(2)}/${(total/1048576).toFixed(2)} МБ)`;

            // audio (finished)
            } else {
                if(isCompleteMP3 === "true"){
                    document.getElementById(`progress_bar${num}`).style.width = "100%";
                    document.getElementById(`progress_bar_num${num}`).innerHTML = "100%";
                }
            }

        // subtitles
        } else {
            document.getElementById(`progress_bar${num}`).style.width = "100%";
            document.getElementById(`progress_bar_num${num}`).innerHTML = `100%`; 
        }
    }
});

// Save info about downloads on close
win.on("close", function () {
    if(document.getElementById("download_list").innerHTML !== ""){
        const progressConts = document.querySelectorAll(".progress-cont");
        let arrDownloads = [];
        for (const item of progressConts) {
            const num = item.dataset.num;
            // pause download if in process
            if (document.getElementById(`progress_btn_pause${num}`).value === "OFF") {
                pauseDownload(num, false);
            }

            let info = {
                num: num,
                url: document.getElementById(`progress_name${num}`).dataset.url,
                folderPath: document.getElementById(`progress_name${num}`).dataset.folderPath,
                downloadName: document.getElementById(`progress_name${num}`).dataset.downloadName,
                container: document.getElementById(`progress_name${num}`).dataset.container,
                itag: document.getElementById(`progress_name${num}`).dataset.itag,
                // downloaded: downloaded,
                total: Number(document.getElementById(`progress_btn_pause${num}`).dataset.total),
                mp3: document.getElementById(`progress_name${num}`).dataset.mp3,
                finishedmp3: document.getElementById(`progress_name${num}`).dataset.finishedmp3,
                completeMP3: document.getElementById(`progress_name${num}`).dataset.completemp3,
                bitrate: document.getElementById(`progress_name${num}`).dataset.bitrate,
                subtitles: document.getElementById(`progress_name${num}`).dataset.subtitles,
            };
            arrDownloads.push(info);
        }
        createFolders();
        const data = JSON.stringify(arrDownloads);
        fs.writeFileSync(dataJSON, data);
    }
    fs.emptyDir(tempRubbish, err => { if(err){ return console.error(err);}})    
    setTimeout(() => {
        this.close(true); // then close it forcefully
    }, 500);
});
/************************/

/******* HOT KEYS *******/
window.addEventListener('keyup', function (event) {
    // Copy link from buffer
    if(event.code == "ArrowUp" || event.code == "Space"){
        processInputField("url", "copy_link");

    // Analyze
    } else if(event.key == "Enter"){
        checkInternet(analyze);
    }
  });
  /************************/