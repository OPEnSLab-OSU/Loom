
inlets = 1;
outlets = 3;

// These might be tuples later so that there is both
// a word and a number encoding
var cmd_dict = {
	"If" : "if",
	"Else" : "else",
	"Endif" : "endif",
	"Not" : "not",
	"Or" : "or",
	"And" : "and",
	"Nor" : "nor",
	"Nand" : "nand",
	"Xor": "xor",
	"Add": "+",
	"Sub": "-",
	"Multiply": "*",
	"Divide": "/",
	"Increment": "++",
	"Decrement": "--",
	"Power": "^",
	"Square": "sqr",
	"==": "==",
	"!=": "!=",
	"<": "<",
	">": ">",
	"<=": "<=",
	">=": ">=",	
	"Pop": "pop",
	"Duplicate": "dup",
	"Print": "printStack",
	"Done": "done"
}

function list() {
	
	script = "";
	
	for (i = 0; i < arguments.length; i++) {
		switch(typeof arguments[i]) {
			case 'string':
				if ( (arguments[i].substring(0, 4) == "Load" ) || (arguments[i].substring(0, 5) == "Store" ) )  {
					outlet(0, arguments[i].toLowerCase());
					script += arguments[i].toLowerCase();
				} else if ( !(arguments[i] in cmd_dict) ) {
					outlet(0, arguments[i]);
					script += arguments[i];
				} else {
					outlet(0, cmd_dict[arguments[i]]);
					script += cmd_dict[arguments[i]];
				}
				break;
			case 'number':
				outlet(0, arguments[i]);
				script += arguments[i].toFixed(4);
				break;
			default:
				outlet(1, arguments[i]);
				break;	
		}
		script += ' ';
	}
	outlet(2, script);
}