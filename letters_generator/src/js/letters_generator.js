/* 
	Letters generator for LED matrix web app
	This app is part of animate LED matrix 
	Designed and coded by Hesham El-sewify (Lehkeda)
	https://www.facebook.com/H.ElSewify
	https://github.com/Lehkeda
	27 April 2018
 */
var letter=[ [0,0,0,0,0,0,0,0],
		 	 [0,0,0,0,0,0,0,0],
			 [0,0,0,0,0,0,0,0],
			 [0,0,0,0,0,0,0,0],
			 [0,0,0,0,0,0,0,0],
			 [0,0,0,0,0,0,0,0],
			 [0,0,0,0,0,0,0,0],
		 	 [0,0,0,0,0,0,0,0] ];

function change_dot_img(node,state,x,y){
 	if(state =="off"){
 		node.getAttributeNode("src").value="./src/img/led_on.svg";
 		node.getAttributeNode("onclick").value="change_dot_img(this,'on',"+x+","+y+")";
 		letter[y-1][x-1]=1;
 	}if(state =="on"){
 		node.getAttributeNode("src").value="./src/img/led_off.svg";
 		node.getAttributeNode("onclick").value="change_dot_img(this,'off',"+x+","+y+")";
 		letter[y-1][x-1]=0;
 	}			
}

function reset_matrix(){
	//reset matrix dots 
	var x=document.getElementsByClassName("LED_dot");
	for (var i =0; i <  x.length; i++) {
 		x[i].getAttributeNode("src").value="./src/img/led_off.svg";
 		x[i].getAttributeNode("onclick").value.replace("on","off");
	}
	//reset letter array
	for(var i=0; i<8; i++){
		for (var x=0; x<8; x++) {
			letter[i][x]=0;
		}
	}
}

function invert_matrix(){
	var x=document.getElementsByClassName("LED_dot");
	//inverse matrix dots
	for (var i =0; i <  x.length; i++) {
		if(x[i].getAttributeNode("src").value=="./src/img/led_off.svg"){
	 		x[i].getAttributeNode("src").value="./src/img/led_on.svg";
	 		x[i].getAttributeNode("onclick").value.replace("off","on");
		}else{
	 		x[i].getAttributeNode("src").value="./src/img/led_off.svg";
	 		x[i].getAttributeNode("onclick").value.replace("on","off");
		}

	}
	//inverse letter array
	for(var i=0; i<8; i++){
		for (var x=0; x<8; x++) {
			if(letter[i][x]) 
				letter[i][x]=0;
			else
				letter[i][x]=1;
		}
	}

}

function generate_letter(){
	var letter_name=document.getElementById("letter_name").value;

		//remove all spaces from letter name
	var letter_name_value=letter_name.replace(/\s/g, "");
	
	if(letter_name_value!=""){
		var output_box=document.getElementById("output_result");
		output_box.value="";
		output_box.value+="// Letter declaration: \n";
		output_box.value+="extern char "+letter_name_value+"[8][8];\n\n";
		output_box.value+="// Letter definition: \n";
		output_box.value+="char ";
		output_box.value+=letter_name_value+"[8][8]";
		output_box.value+="={\n";
		for(var i=0; i<8; i++){
			for (var x=0; x<8; x++) {
				if(x == 0) output_box.value+="{";
				output_box.value+=letter[i][x];
				if(x != 7) output_box.value+=",";
			}
			output_box.value+="}";
			if(i != 7) output_box.value+=",\n";
			else output_box.value+=" };";
		}
	}
}

function copy_letter(){
	var output_box=document.getElementById("output_result");
	if(output_box.value!=""){
		output_box.select();
		document.execCommand("copy");
	}
}
