import {Application} from "pixi.js";
import * as PIXI from "pixi.js";

const app = new Application();

await app.init({
    width: 400,
    height: 300,
    backgroundColor: 0xffffff,
});
document.getElementById('app').appendChild(app.canvas);

const rectangle = new PIXI.Graphics();
const rectWidth = 200;
const rectHeight = 100;
rectangle.fill(0x00dd00);
rectangle.rect(0, 0, rectWidth, rectHeight);
rectangle.fill();

rectangle.x = app.screen.width / 2;
rectangle.y = app.screen.height / 2;
rectangle.pivot.x = rectWidth / 2;
rectangle.pivot.y = rectHeight / 2;

app.stage.addChild(rectangle);

app.ticker.add(() => {
    rectangle.rotation += 0.01;
});
