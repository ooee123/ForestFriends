var dimension = {width: 24, length: 12}

var fontSize = 1;
var canvas = document.getElementById('main');
var context = canvas.getContext('2d');
var textBox = []
var scale = 20;

var getDimension = function (varName) {
   return function () {
      dimension[varName] = document.getElementById(this.id).value
      updateRect(0);
   }

}

var changeWHF = function() {
   //alert(fontSize + " " + textBox.length);
   for (var i = 0 ; i < textBox.length; i++) {
      updateEverything(i+1)();
   }
   
}

var updateEverything = function (canvasNumber) {
   return function () {
      var x = document.getElementById("x" + canvasNumber).value;
      var y = document.getElementById("y" + canvasNumber).value;
      var text = document.getElementById("text" + canvasNumber).value.toUpperCase();
      var canvas = textBox[canvasNumber - 1]
      if (dimension["length"] - 2 <= parseInt(document.getElementById("fontSize").value)) {
         dimension["length"] = parseInt(document.getElementById("fontSize").value)+2;
         document.getElementById("yCoord").value = dimension["length"] 
         updateRect(canvasNumber)
         //alert("here " + dimension["length"])
      }
      var width = dimension["width"]
      var length = dimension["length"]
      
      if (x == "" || x < 1) {
         x = 1;
         document.getElementById("x" + canvasNumber).value = 1
      }
      var height = parseInt(document.getElementById("fontSize").value);
      if (y == "" || (y < 2 || height >= y)) {
         if (height != 1) {
            y = height + 1
         } 
         else {
            y = 2;
            document.getElementById("y" + canvasNumber).value = y
            context.clearRect(0, 0, canvas.width*scale, canvas.height*scale)
         }
      }
      else if (y != "" && y > length - 1) {
         y = length - 1;
      }
      document.getElementById("y" + canvasNumber).value = y

      // check text for arrows
      text = checkArrow(text)

      canvas.x = x
      canvas.y = y
      canvas.text = text;

      var contextTemp = canvas.getContext("2d")
      contextTemp.font = 'normal ' + document.getElementById("fontSize").value*22 + 'pt hwygoth'; 
      contextTemp.fillText(text, x*scale, y*scale);
      canvas.border = (width * scale - 20 - x * scale) - contextTemp.measureText(text).width
      contextTemp.clearRect(0,0,canvas.width*scale, canvas.height*scale)

      var context = canvas.getContext("2d")
      context.clearRect(0, 0, canvas.width*scale, canvas.height*scale)
      context.rect(0, 0, width*scale, length*scale);
      canvas.width = width*scale;
      canvas.height = length*scale;
      if (canvas.border < 0) {
         // red
         context.fillStyle = 'red'
         alert("Your text is not within a 1 inch border. \nPlease adjust your text to be within 1 inch.")
      }
      context.font = 'normal ' + document.getElementById("fontSize").value*22 + 'pt hwygoth'; // ** take a look at this
      context.fillText(text, x*scale, y*scale);
   }
}

var checkArrow = function(string) {
   string = string.replace("^", "↑")
   string = string.replace(">", "→")
   string = string.replace("<", "←")
   return string
}
var updateRect = function (number) {
   if (dimension["width"] < 12) {
      document.getElementById("xCoord").value = 12
      dimension["width"] = 12;
   } 

   if (dimension["length"] < 12) {
      document.getElementById("yCoord").value = 12
      dimension["length"] = 12;
   }
   
   
   if (dimension["width"] > 48) {
      document.getElementById("xCoord").value = 48
      dimension["width"] = 48;
   }
   if (dimension["length"] > 24) {
      document.getElementById("yCoord").value = 24
      dimension["length"] = 24;
   }
   
   
   var width = dimension["width"]
   var length = dimension["length"]
   //alert(width + " " + length);
   context.clearRect(0, 0, canvas.width*scale, canvas.height*scale)
   context.beginPath();
   context.rect(0, 0, width*scale, length*scale);
   context.fillStyle = 'BurlyWood';
   context.fill();
   context.lineWidth = 1;
   context.strokeStyle = 'black';
   context.stroke();
   if (!number) {
      changeWHF();
   }
};

var createCanvas = function () {
   var newCanvas = document.createElement('canvas')
   
   textBox[textBox.length] = newCanvas;
   
   newCanvas.id = "txt" + textBox.length;
   newCanvas.width = dimension.width * scale;
   newCanvas.height = dimension.length * scale; // can be changed
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
      
   }
   
   textBoxElem.appendChild(createTextBoxDiv())
   
   document.getElementById('canvas').appendChild(newCanvas);
}

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
   //xInput.addEventListener("keyup", input);
   xInput.addEventListener("focusout", input);
   container.appendChild(xInput)
   
   var yInput = document.createElement("input");
   yInput.type = "number"
   yInput.min = "0"
   yInput.max = "24"
   yInput.id = "y" + textBox.length
   //yInput.addEventListener("keyup", input);
   yInput.addEventListener("focusout", input);
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



// Sample Board with Text and Board Dimensions
var createCanvasSample = function () {
   updateRectSample()
   var newCanvas = document.createElement('canvas')
   
   textBox[textBox.length] = newCanvas;
   
   newCanvas.id = "txt" + textBox.length;
   newCanvas.width = dimension.width * scale;
   newCanvas.height = dimension.length * scale; // can be changed
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
      
   }

   
   textBoxElem.appendChild(createTextBoxDiv())
   
   document.getElementById('canvas').appendChild(newCanvas);

   var xElem = document.getElementById("x" + textBox.length);
   var yElem = document.getElementById("y" + textBox.length);
   var textElem = document.getElementById("text" + textBox.length);
   xElem.value = 3;
   yElem.value = 3;
   textElem.value = "FOREST FRIENDS";
   updateEverything(textBox.length)();
}

var updateRectSample = function () {
   dimension.width = 36; // 36
   dimension.length = 16; // 16
   context.clearRect(0, 0, dimension.width*scale, dimension.height*scale)
   context.beginPath();
   context.rect(0, 0, dimension.width*scale, dimension.height*scale);
   context.fillStyle = 'BurlyWood';
   context.fill();
   context.lineWidth = 1;
   context.strokeStyle = 'black';
   context.stroke();
   
   document.getElementById("xCoord").value = 36
   document.getElementById("yCoord").value = 16
   document.getElementById("fontSize").value = 1
   changeWHF();
   updateRect(0);
};



/* Assigning listeners to the length and width number fields */
//document.getElementById("yCoord").addEventListener("keyup", getDimension("length"))
//document.getElementById("xCoord").addEventListener("keyup", getDimension("width"))
document.getElementById("yCoord").addEventListener("focusout", getDimension("length"))
document.getElementById("xCoord").addEventListener("focusout", getDimension("width"))
//document.getElementById("fontSize").addEventListener("keyup", changeWHF)
document.getElementById("fontSize").addEventListener("focusout", changeWHF)
document.getElementById("newTextBox").addEventListener("click", createCanvas)
document.getElementById("sampleBoard").addEventListener("click", createCanvasSample)
document.getElementById("newTextBoxSampleBoard").addEventListener("click", updateRectSample)


updateRect(0)
changeWHF()

var checkRed = function () {
   for (var i = 0; i < textBox.length; i++) {
      if (textBox[i].border < 0) {
         //alert("red")
         alert("Please make sure your text is within a 1 inch border.\n")
         return false;
      }
   }
   return true;
}

function saveTextAsFile() {
   
   var textToWrite = "";
   textToWrite += document.getElementById("yCoord").value + "\n"
   textToWrite += document.getElementById("xCoord").value + "\n"
   textToWrite += document.getElementById("fontSize").value + "\n"
   
   for (var i = 0; i < textBox.length; i++) {
      var c = textBox[i]
      if (c.x && c.y && c.text) {
         textToWrite += (c.x + "\n" + c.y + "\n" + c.text + "\n" /*+ c.font + "\n"*/)
      }
   }
   
   var textFileAsBlob = new Blob([textToWrite], {
                                 type: 'text/plain'
                                 });
   var fileNameToSaveAs = document.getElementById("inputFileNameToSaveAs").value;
   if (fileNameToSaveAs == "") {
      alert("Please Enter a Filename");
      return;
   }
   
   var downloadLink = document.createElement("a");
   downloadLink.download = fileNameToSaveAs// + ".cpff";
   downloadLink.innerHTML = "Download File";
   
   if (checkRed()) {
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
   }
   downloadLink.click();
}

/* Begin Parsing Methods */
var Text = function(x, y, text) {
   this.x = x
   this.y = y
   this.text = text
}

var Dimension = function(width, height, letterHeight) {
   this.width = width
   this.height = height
   this.letterHeight = height
}

var cleanString = function(s) {
   var regex = /(<([^>]+)>)/ig
   return s.replace(regex, "")
}

document.getElementById('upload').onchange = function(){
   var file = this.files[0];
   
   
   var reader = new FileReader();
   reader.onload = function(progressEvent){
      // By lines
      var lines = this.result.split('\n');
      
      for (var n = 0; n < lines.length; n++)
      {
         lines[n] = cleanString(lines[n])
      }
      
      //var dimensions = new Dimension(lines[0], lines[1], lines[2]);
      document.getElementById("yCoord").value = dimension["length"] = lines[0];
      document.getElementById("xCoord").value = dimension["width"] = lines[1];
      document.getElementById("fontSize").value = lines[2];
      updateRect(0);
      var texts = [];
      
      for(var line = 3; line + 3 < lines.length; line = line + 3){
         //texts[texts.length] = new Text(lines[line], lines[line + 1], lines[line + 2]);
         var index = line / 3
         if (textBox.length < index)
         {
            createCanvas();
         }
         var xElem = document.getElementById("x" + index);
         var yElem = document.getElementById("y" + index);
         var textElem = document.getElementById("text" + index);
         xElem.value = lines[line];
         yElem.value = lines[line + 1];
         textElem.value = lines[line + 2];
         updateEverything(textBox.length)();
      }
      
      return this.result
   };
   reader.readAsText(file);
};
