/**
 * BitMatrix - Dot Matrix Display Controller
 * Copyright 2020 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

const fs = require("fs");
const path = require("path");

const { createCanvas, loadImage } = require("canvas");

const execPath = process.argv[1];
const imagePath = process.argv[2];

if (!imagePath) {
    console.error("Image path not specified.");
    process.exit(1);
}

const imageName = path.basename(imagePath);
const baseName = path.basename(imagePath, path.extname(imagePath));
const basePath = path.dirname(imagePath);
const dataFileBase = `data_${baseName.toLowerCase()}`;

console.log("Loading image from '%s'", imagePath);

loadImage(imagePath).then(image => {

    const width = image.width;
    const height = image.height;
    const size = width * height;

    console.log("Image loaded, width = %s, height = %s", width, height);    

    if (width !== 128 || (height != 128 && height != 64)) {
        console.error("Invalid image size. Must be 128 x 128/64 but is %s x %s", width, height);
        process.exit(1);
    }

    const canvas = createCanvas(width, height);
    const context = canvas.getContext("2d");
    
    context.drawImage(image, 0, 0);

    const imageData = context.getImageData(0, 0, width, height);
    const pixels = imageData.data;
    
    const binaryData = new Uint8Array(size / 8);

    const source = [
        "/**",
        " * Auto-generated C data file",
        ` * Source: ${imagePath}`,
        " */",
        "",
        `#include "${dataFileBase}.h"`,
        "",
        `const unsigned char ${dataFileBase}[${size / 8}] = {`, 
    ];

    for (let yy = 0; yy < size; yy += width) {
        const line = [];
        for (let x = 0; x < width; x += 8) {
            let byte = 0;
            for (let bit = 0; bit < 8; ++bit) {
                const pixel = pixels[(yy + x + bit) * 4];
                if (pixel > 0) {
                    byte += (1 << bit);
                }
            }

            binaryData[(yy + x) / 8] = byte;

            const entry = "0x" + ("00" + byte.toString(16)).substr(-2);
            line.push(entry);
        }
        source.push(`    ${line.join(", ")},`);
    }

    source.push(
        "};",
        "",
    );

    const binFilePath = path.resolve(basePath, baseName + ".bin");
    fs.writeFileSync(binFilePath, binaryData);

    const implFilePath = path.resolve(basePath, dataFileBase + ".cpp");
    fs.writeFileSync(implFilePath, source.join("\n"));

    const header = [
        "/**",
        " * Auto-generated C data file",
        ` * Source: ${imagePath}`,
        " */",
        "",
        `#ifndef _${dataFileBase.toUpperCase()}_H`,
        `#define _${dataFileBase.toUpperCase()}_H`,
        "",
        `#define ${dataFileBase.toUpperCase()}_SIZE ${size / 8}`,
        "",
        `extern const unsigned char ${dataFileBase}[${size / 8}];`,
        "",
        `#endif // _${dataFileBase.toUpperCase()}_H`,
        "",
    ];

    const headerFilePath = path.resolve(basePath, dataFileBase + ".h");
    fs.writeFileSync(headerFilePath, header.join("\n"));


}).catch(error => {
    console.log("Error while loading image: %s", error);
    process.exit(1);
});


 