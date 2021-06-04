// called when the client connects
function onConnect() {
    // Once a connection has been made, make a subscription and send a message.
    let topic = document.forms["connectionForm"]["topic"].value
    console.log("onConnect");
    client.subscribe(topic);
    message = new Paho.MQTT.Message("Hello from web");
    message.destinationName = topic;
    client.send(message);
}

// called when the client loses its connection
function onConnectionLost(responseObject) {
    if (responseObject.errorCode !== 0) {
        console.log("onConnectionLost:" + responseObject.errorMessage);
    }
}

// called when a message arrives
function onMessageArrived(message) {
    console.log("Message:\t" + message.payloadString);

    console.log("Time:\t\t" + Date.now());
    console.log("delay:\t\t" + (Date.now() - parseInt(message.payloadString)) + "ms");

}

function send(sendMessage) {
    let topic = document.forms["connectionForm"]["topic"].value


    message = new Paho.MQTT.Message(JSON.stringify(sendMessage));
    message.destinationName = topic;
    client.send(message);

    document.getElementById("lastMessage").innerHTML = syntaxHighlight(JSON.stringify(sendMessage, undefined, 2))


}

var coll = document.getElementsByClassName("collapsible");
var i;

for (i = 0; i < coll.length; i++) {
    coll[i].addEventListener("click", function () {
        this.classList.toggle("active");
        var content = this.nextElementSibling;
        if (content.style.maxHeight) {
            content.style.maxHeight = null;
        } else {
            content.style.maxHeight = content.scrollHeight + "px";
        }
    });
}

function connect() {
    let input = document.forms["connectionForm"]

    let ip = input["ip"].value
    let port = parseInt(input["port"].value)
    let topic = input["topic"].value;

    console.log(ip + "\t" + typeof (ip))
    console.log(port + "\t" + typeof (port))
    console.log(topic + "\t" + typeof (topic))
    // Create a client instance
    client = new Paho.MQTT.Client(ip, port, "/" + topic, "WebClient-LetsHopeOnlyOneConnectionExists");

    // set callback handlers
    client.onConnectionLost = onConnectionLost;
    client.onMessageArrived = onMessageArrived;

    // connect the client
    client.connect({ onSuccess: (onConnect) });
}

function updateInputType() {
    let button = document.getElementById("toggle")
    console.log(button.checked)

    if (button.checked) {
        document.getElementById("input").innerHTML = "";
    } else {
        document.getElementById("input").innerHTML = "";
    }
}

function syntaxHighlight(json) {
    json = json.replace(/&/g, '&amp;').replace(/</g, '&lt;').replace(/>/g, '&gt;');
    return json.replace(/("(\\u[a-zA-Z0-9]{4}|\\[^u]|[^\\"])*"(\s*:)?|\b(true|false|null)\b|-?\d+(?:\.\d*)?(?:[eE][+\-]?\d+)?)/g, function (match) {
        var cls = 'number';
        if (/^"/.test(match)) {
            if (/:$/.test(match)) {
                cls = 'key';
            } else {
                cls = 'string';
            }
        } else if (/true|false/.test(match)) {
            cls = 'boolean';
        } else if (/null/.test(match)) {
            cls = 'null';
        }
        return '<span class="' + cls + '">' + match + '</span>';
    });
}