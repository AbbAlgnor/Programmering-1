let lastSend = 0;
let lastMessage = [];

window.addEventListener("gamepadconnected", function (e) {
    console.log("Gamepad connected at index %d: %s. %d buttons, %d axes.",
        e.gamepad.index, e.gamepad.id,
        e.gamepad.buttons.length, e.gamepad.axes.length);
    loop()
});
window.addEventListener("gamepaddisconnected", function (e) {
    console.log("Gamepad disconnected from index %d: %s",
        e.gamepad.index, e.gamepad.id);
});
var gamepads = {};

function gamepadHandler(event, connecting) {
    var gamepad = event.gamepad;
    // Note:
    // gamepad === navigator.getGamepads()[gamepad.index]

    if (connecting) {
        gamepads[gamepad.index] = gamepad;
    } else {
        delete gamepads[gamepad.index];
    }
}

window.addEventListener("gamepadconnected", function (e) { gamepadHandler(e, true); }, false);
window.addEventListener("gamepaddisconnected", function (e) { gamepadHandler(e, false); }, false);

function loop() {
    var gamepads = navigator.getGamepads ? navigator.getGamepads() : (navigator.webkitGetGamepads ? navigator.webkitGetGamepads : []);
    if (!gamepads) {
        return;
    }

    var gp = gamepads[0];

    let message = {
        Forward: Math.round((gp.axes[5] / 2 + 0.5) * 1024),
        Backward: Math.round((gp.axes[2] / 2 + 0.5) * 1024),
        Turning: Math.round(gp.axes[0] * -90)
    }

    console.log(message);

    if (Date.now() >= (lastSend + 10)) {

        console.error(message.is)

        if (JSON.stringify(message) !== JSON.stringify(lastMessage)) {
            send(message);

            lastSend = Date.now();
            console.warn("sent!");
            lastMessage = message;
            console.warn(message);
            console.warn(lastMessage);
        }
    }
    start = requestAnimationFrame(loop)
}