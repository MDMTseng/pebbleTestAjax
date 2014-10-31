var OKss=0;
function ssss(){
  var req = new XMLHttpRequest();
  req.open('GET', 'http://mdm.noip.me:5213/?callback=ssss', true);

  
    req.send(null);
}

Pebble.addEventListener("ready", function(e) {

  
  
  
  Pebble.addEventListener("appmessage", function(e) {
    ssss();
    //console.log("addEventListener"+ e.payload[0]);
    //Pebble.sendAppMessage({ "0": e.payload[0]});
  });
  
});