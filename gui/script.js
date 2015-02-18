
   var dimension = {width: 24, length: 12}
   var fontSize = 1;
   var canvas = document.getElementById('main');
   var context = canvas.getContext('2d');
   var textBox = []
   
   var getDimension = function (varName) {
      return function () {
         dimension[varName] = document.getElementById(this.id).value
         updateRect();
      }
   }

var changeFont = function() {
   //alert(fontSize + " " + textBox.length);
   for (var i = 0 ; i < textBox.length; i++) {
      //alert("that")
      updateEverything(i+1)();
      //alert("this")
   }

}

var updateEverything = function (canvasNumber) {
   return function () {
      
      var x = document.getElementById("x" + canvasNumber).value;
      var y = document.getElementById("y" + canvasNumber).value;
      var text = document.getElementById("text" + canvasNumber).value;
      var canvas = textBox[canvasNumber - 1]
      var width = dimension["width"]
      var length = dimension["length"]
      canvas.x = x
      canvas.y = y
      canvas.text = text
      var context = canvas.getContext("2d")
      context.clearRect(0, 0, canvas.width*20, canvas.height*20)
      context.rect(0, 0, width*20, length*20);
      context.width = canvas.width*20;
      context.height = canvas.height*20;
      context.font = 'normal ' + document.getElementById("fontSize").value* 28 + 'pt Calibri'; // ** take a look at this
      context.fillText(text, x*20, y*20);
   }
}

var updateRect = function () {
   var width = dimension["width"]
   var length = dimension["length"]
   //alert(width + " " + length);
   context.clearRect(0, 0, canvas.width*20, canvas.height*20)
   context.beginPath();
   context.rect(0, 0, width*20, length*20);
   context.fillStyle = 'BurlyWood';
   context.fill();
   context.lineWidth = 1;
   context.strokeStyle = 'black';
   context.stroke();
};


var updateRectSample = function () {
   dimension.width = 36; // 36
   dimension.length = 16; // 16
   context.clearRect(0, 0, dimension.width*20, dimension.height*20)
   context.beginPath();
   context.rect(0, 0, dimension.width*20, dimension.height*20);
   context.fillStyle = 'sienna ';
   context.fill();
   context.lineWidth = 1;
   context.strokeStyle = 'black';
   context.stroke();
   
   
   document.getElementById("xCoord").value = 36
   document.getElementById("yCoord").value = 16
   
   updateRect();
};

var createTextBoxDiv = function () {
   var container = document.createElement("div")
   
   container.dimension = {width: 0, height: 0}
   container.x = 0
   container.y = 0
   container.font = fontSize;
   
   var input = updateEverything(textBox.length)
   
   var xInput = document.createElement("input");
   xInput.type = "number"
   xInput.min = "0"
   xInput.max = "48"
   xInput.id = "x" + textBox.length
   xInput.addEventListener("keyup", input);
   container.appendChild(xInput)
   
   var yInput = document.createElement("input");
   yInput.type = "number"
   yInput.min = "0"
   yInput.max = "24"
   yInput.id = "y" + textBox.length
   yInput.addEventListener("keyup", input);
   container.appendChild(yInput)
   
   var textInput = document.createElement("input");
   textInput.type = "text"
   textInput.id = "text" + textBox.length
   textInput.addEventListener("keyup", input);
   container.appendChild(textInput)
   
   /*
   var fontInput = document.createElement("input");
   fontInput.type = "number"
   fontInput.min = "0"
   fontInput.id = "font" + textBox.length
   fontInput.addEventListener("keyup", input);
   container.appendChild(fontInput)
   */
   container.id = "div" + textBox.length
   return container
}

/*
var createTextBoxDivSample = function () {
   
   var container = document.createElement("div")
   
   container.dimension = {width: 0, height: 0}
   container.x = 0
   container.y = 0
   
   var input = updateEverything(textBox.length)
   
   var xInput = document.createElement("input");
   xInput.type = "number"
   xInput.min = "0"
   xInput.id = "x" + textBox.length
   xInput.value = 12
   container.appendChild(xInput)
   
   var yInput = document.createElement("input");
   yInput.type = "number"
   yInput.min = "0"
   yInput.id = "y" + textBox.length
   yInput.value = 24
   container.appendChild(yInput)
   
   var textInput = document.createElement("input");
   textInput.type = "text"
   textInput.id = "text" + textBox.length
   textInput.value = "Hello World"
   container.appendChild(textInput)
 
   container.id = "div" + textBox.length
   updateEverything(textBox.length);
   
   return container
}
*/

var createCanvas = function () {
   var newCanvas = document.createElement('canvas')
   
   textBox[textBox.length] = newCanvas;
   
   newCanvas.id = "txt" + textBox.length;
   newCanvas.width = dimension.width * 20;
   newCanvas.height = dimension.length * 20; // can be changed
   //newCanvas.width = 960;
   //newCanvas.height = 480; // can be changed
   newCanvas.style.zIndex = 2;
   newCanvas.style.position = "absolute";
   
   var textBoxElem = document.getElementById('textbox')
   if (textBox.length == 1) {
      
      var label = document.createElement('input')
      label.type = "text"
      label.value = "X Coordinate (in)"
      label.readOnly = "true"
      textBoxElem.appendChild(label)
      
      var ylabel = document.createElement('input')
      ylabel.type = "text"
      ylabel.value = "Y Coordinate (in)"
      ylabel.readOnly = "true"
      textBoxElem.appendChild(ylabel)
      
      var zlabel = document.createElement('input')
      zlabel.type = "text"
      zlabel.value = "Text"
      zlabel.readOnly = "true"
      textBoxElem.appendChild(zlabel)

      /*
      var flabel = document.createElement('input')
      flabel.type = "text"
      flabel.value = "Font Size"
      flabel.readOnly = "true"
      textBoxElem.appendChild(flabel)
      */
   }
   
   textBoxElem.appendChild(createTextBoxDiv())
   
   document.getElementById('canvas').appendChild(newCanvas);
}

var createCanvasSample = function () {
   var newCanvas = document.createElement('canvas')
   
   textBox[textBox.length] = newCanvas;
   
   newCanvas.id = "txt" + textBox.length;
   newCanvas.width = dimension.width * 20;
   newCanvas.height = dimension.length * 20; // can be changed
   newCanvas.style.zIndex = 2;
   newCanvas.style.position = "absolute";
   
   var textBoxElem = document.getElementById('textbox')
   if (textBox.length == 1) {
      
      var label = document.createElement('input')
      label.type = "text"
      label.value = "X Coordinate"
      label.readOnly = "true"
      textBoxElem.appendChild(label)
      
      var ylabel = document.createElement('input')
      ylabel.type = "text"
      ylabel.value = "Y Coordinate"
      ylabel.readOnly = "true"
      textBoxElem.appendChild(ylabel)
      
      var zlabel = document.createElement('input')
      zlabel.type = "text"
      zlabel.value = "Text"
      zlabel.readOnly = "true"
      textBoxElem.appendChild(zlabel)
      
      /*
      var flabel = document.createElement('input')
      flabel.type = "text"
      flabel.value = "Font Size"
      flabel.readOnly = "true"
      textBoxElem.appendChild(flabel)
 */
   }
   
   textBoxElem.appendChild(createTextBoxDivSample())
   
   document.getElementById('canvas').appendChild(newCanvas);
}

function saveTextAsFile() {
   var textToWrite = "";
   textToWrite += document.getElementById("xCoord").value + "\n"
   textToWrite += document.getElementById("yCoord").value + "\n"
   textToWrite += document.getElementById("fontSize").value + "\n"
   
   for (var i = 0; i < textBox.length; i++) {
      var c = textBox[i]
      textToWrite += (c.x + "\n" + c.y + "\n" + c.text + "\n" /*+ c.font + "\n"*/)
   }
   
   var textFileAsBlob = new Blob([textToWrite], {
                                 type: 'text/plain'
                                 });
                                 var fileNameToSaveAs = document.getElementById("inputFileNameToSaveAs").value;
                                 
                                 
                                 var downloadLink = document.createElement("a");
                                 downloadLink.download = fileNameToSaveAs;
                                 downloadLink.innerHTML = "Download File";
                                 if (window.webkitURL != null) {
                                    // Chrome allows the link to be clicked
                                    // without actually adding it to the DOM.
                                    downloadLink.href = window.webkitURL.createObjectURL(textFileAsBlob);
                                 } else {
                                    // Firefox requires the link to be added to the DOM
                                    // before it can be clicked.
                                    downloadLink.href = window.URL.createObjectURL(textFileAsBlob);
                                    downloadLink.onclick = destroyClickedElement;
                                    downloadLink.style.display = "none";
                                    document.body.appendChild(downloadLink);
                                 }
                                 
                                 downloadLink.click();
}


/* Assigning listeners to the length and width number fields */
document.getElementById("yCoord").addEventListener("keyup", getDimension("length"))
document.getElementById("xCoord").addEventListener("keyup", getDimension("width"))
document.getElementById("fontSize").addEventListener("keyup", changeFont)
document.getElementById("newTextBox").addEventListener("click", createCanvas)
//document.getElementById("newTextBoxSampleText").addEventListener("click", createCanvasSample)
document.getElementById("newTextBoxSampleBoard").addEventListener("click", updateRectSample)



updateRect()
