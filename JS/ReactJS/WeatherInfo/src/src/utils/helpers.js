const getTomorrow = (time) => {
    return new Date(time).setHours(0,0,0,0)>new Date().setHours(0,0,0,0);
  }

const formatDate = (time, type) => {
    const inputDate = new Date(time);
    let outputDate;

    if(type === 'hourly'){
        outputDate = inputDate.toLocaleTimeString('ru-ru', { hour: "2-digit", minute: "2-digit", hour12: false});
    } else if(type === 'daily'){
        outputDate = inputDate.toLocaleDateString('en-us', { weekday: "short", month: "short", day: "numeric"});
    } else if(type === 'today'){
        outputDate = new Date().toLocaleDateString('en-us', { year: "numeric", month: "short", day: "numeric"});
    } 

    return outputDate;
}

const getTemp = (temp) => {
    if(temp > 0){
      return `+${Math.round(temp).toString()}`;
    } else if(temp < 0){
      return `-${Math.round(temp).toString()}`;
    }
    return Math.round(temp).toString();
  }

module.exports = {
    getTomorrow,
    formatDate,
    getTemp,
}