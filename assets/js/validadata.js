// //Versao 1.0.0 /Versao
function FormataHora(Texto,event)
	{
	if(navigator.appName.indexOf("Netscape")!= -1) 
		{
		tecla= event.which; 
		}
	else 
		{
		tecla= event.keyCode; 
		}
	key = String.fromCharCode(tecla); 
	if (key == 8 || tecla == 47)
		{
		return
		}	
	if ((Texto.value.length  == 1) & (key == ":"))
		{
		Texto.value = "0" + Texto.value;
		}
	if ((Texto.value.length == 2) & (key != ":"))
		{
		if (Texto.value.substr(Texto.value.length -1,1) != ":")
			{
			Texto.value = Texto.value + ":";
			}
		}
	}

function FormataData(Obj, event)
	{
	tecla= event.keyCode; 
	key = String.fromCharCode(tecla); 
	if (key == 8 || tecla == 47)
		{
		return
		}
	if (Obj.value.length == 2 | Obj.value.length == 5 )
		{
		if (Obj.value.substr(Obj.value.length -1,1) != "/")
			{
			Obj.value = Obj.value + "/";
			}
		}
	}

function isDigit(theDigit) 
{ 
	var digitArray = new Array('0','1','2','3','4','5','6','7','8','9'),j; 
	for (j = 0; j < digitArray.length; j++) 
		{
		if (theDigit == digitArray[j]) 
		return true 
		} 
	return false 
} 
/*************************************************************************/ 
/*Function name :isPositiveInteger(theString) */ 
/*Usage of this function :test for an +ve integer */ 
/*Input parameter required:thedata=string for test whether is +ve integer*/ 
/*Return value :if is +ve integer,return true */ 
/* else return false */ 
/*function require :isDigit */ 
/*************************************************************************/ 
function isPositiveInteger(theString) 
	{ 
	var theData = new String(theString) 
	if (!isDigit(theData.charAt(0))) 
		{
		if (!(theData.charAt(0)== '+')) 
			{
			return false 
			}
		}
	for (var i = 1; i < theData.length -1; i++) 
		{
		if (!isDigit(theData.charAt(i))) 
			{
			return false 
			}
		}
	return true 
} 

/**********************************************************************/ 
/*Function name :isDate(s,f) */ 
/*Usage of this function :To check s is a valid format */ 
/*Input parameter required:s=input string */ 
/* f=input string format */ 
/* =1,in mm/dd/yyyy format */ 
/* else in dd/mm/yyyy */ 
/*Return value :if is a valid date return 1 */ 
/* else return 0 */ 
/*Function required :isPositiveInteger() */ 
/**********************************************************************/ 
function isDate(s,f) 
	{
	var Data = s.split(" ");	
	var a1=Data[0].split("/"); 
	var a2=Data[0].split("-"); 
	var e=true; 
	if ((a1.length!=3) && (a2.length!=3)) 
		{ 
		e=false; 
		} 
	else 
		{
		if (a1.length==3) 
			var na=a1; 
		if (a2.length==3) 
			var na=a2; 
		if (isPositiveInteger(na[0]) && isPositiveInteger(na[1]) && isPositiveInteger(na[2])) 
			{ 
			if ( f == 1 ) 
				{
				var d=na[1],m=na[0]; 
				} 
			else 
				{
				var d=na[0],m=na[1]; 
				} 
			var y=na[2];
			if ((e) && (y<1000)) 
				e=false 
			if (e) 
				{ 
				v=new Date(m+"/"+d+"/"+y); 			
				if (v.getMonth()!=m-1) 
					e=false; 
				} 
			} 
		else 
			{ 
			e=false; 
			} 
		} 
	if (Data.length > 1)
		{
			hora = Data[1].split(':');
			if (isPositiveInteger(hora[0]) && isPositiveInteger(hora[1]))
				{
				if (hora[0] > 23)
					{
					return false
					}
				if (hora[1] > 59)
					{
					return false;	
					}
				}
			else
				{
				e=false;
				}
		}
	return e 
	} 

function checkDate(data) 
{ 
var s=data;
if (isDate(s,0)) //dd/mm/yyyy format 
	{
	return true; 
	}
else
	{
	return false;
	}
} 

