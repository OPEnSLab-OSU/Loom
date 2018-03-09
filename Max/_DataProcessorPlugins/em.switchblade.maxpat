{
	"patcher" : 	{
		"fileversion" : 1,
		"appversion" : 		{
			"major" : 6,
			"minor" : 1,
			"revision" : 8,
			"architecture" : "x86"
		}
,
		"rect" : [ 0.0, 44.0, 953.0, 618.0 ],
		"bglocked" : 0,
		"openinpresentation" : 1,
		"default_fontsize" : 12.0,
		"default_fontface" : 0,
		"default_fontname" : "Arial",
		"gridonopen" : 0,
		"gridsize" : [ 15.0, 15.0 ],
		"gridsnaponopen" : 0,
		"statusbarvisible" : 2,
		"toolbarvisible" : 1,
		"boxanimatetime" : 200,
		"imprint" : 0,
		"enablehscroll" : 1,
		"enablevscroll" : 1,
		"devicewidth" : 0.0,
		"description" : "",
		"digest" : "",
		"tags" : "",
		"boxes" : [ 			{
				"box" : 				{
					"fontname" : "Optima Regular",
					"fontsize" : 12.0,
					"id" : "obj-27",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 500.0, 52.0, 98.0, 21.0 ],
					"text" : "udpreceive 7477"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Optima Regular",
					"fontsize" : 12.0,
					"id" : "obj-54",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "bang" ],
					"patching_rect" : [ 281.0, 456.0, 50.0, 21.0 ],
					"text" : "del 500"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Optima Regular",
					"fontsize" : 12.0,
					"id" : "obj-50",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "bang" ],
					"patching_rect" : [ 281.0, 426.0, 59.0, 21.0 ],
					"text" : "loadbang"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Optima Regular",
					"fontsize" : 12.0,
					"id" : "obj-49",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"patching_rect" : [ 263.0, 519.0, 59.0, 21.0 ],
					"text" : "route text"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Optima Regular",
					"fontsize" : 12.0,
					"frgb" : 0.0,
					"id" : "obj-47",
					"maxclass" : "textedit",
					"numinlets" : 1,
					"numoutlets" : 4,
					"outlettype" : [ "", "int", "", "" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 263.0, 487.0, 46.0, 25.0 ],
					"text" : "0"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Optima Regular",
					"fontsize" : 12.0,
					"id" : "obj-46",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "int" ],
					"patching_rect" : [ 251.0, 426.0, 32.5, 21.0 ],
					"text" : "+ 1"
				}

			}
, 			{
				"box" : 				{
					"disabled" : [ 0, 0 ],
					"id" : "obj-45",
					"itemtype" : 0,
					"maxclass" : "radiogroup",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 251.0, 371.0, 18.0, 34.0 ],
					"size" : 2,
					"value" : 0
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Optima Regular",
					"fontsize" : 12.0,
					"id" : "obj-44",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"patching_rect" : [ 318.0, 367.5, 52.0, 21.0 ],
					"text" : "gate 2 1"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Optima Regular",
					"fontsize" : 12.0,
					"id" : "obj-42",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 466.0, 228.0, 50.0, 19.0 ],
					"text" : "RIGHT"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Optima Regular",
					"fontsize" : 12.0,
					"id" : "obj-40",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patcher" : 					{
						"fileversion" : 1,
						"appversion" : 						{
							"major" : 6,
							"minor" : 1,
							"revision" : 8,
							"architecture" : "x86"
						}
,
						"rect" : [ 63.0, 251.0, 640.0, 480.0 ],
						"bglocked" : 0,
						"openinpresentation" : 0,
						"default_fontsize" : 12.0,
						"default_fontface" : 0,
						"default_fontname" : "Arial",
						"gridonopen" : 0,
						"gridsize" : [ 15.0, 15.0 ],
						"gridsnaponopen" : 0,
						"statusbarvisible" : 2,
						"toolbarvisible" : 1,
						"boxanimatetime" : 200,
						"imprint" : 0,
						"enablehscroll" : 1,
						"enablevscroll" : 1,
						"devicewidth" : 0.0,
						"description" : "",
						"digest" : "",
						"tags" : "",
						"boxes" : [ 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 12.0,
									"id" : "obj-16",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 6.0, 296.0, 34.0, 18.0 ],
									"text" : "$1 0"
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 12.0,
									"id" : "obj-15",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 40.0, 296.0, 34.0, 18.0 ],
									"text" : "$1 1"
								}

							}
, 							{
								"box" : 								{
									"comment" : "output Macro Sequence",
									"id" : "obj-17",
									"maxclass" : "outlet",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 6.0, 367.0, 25.0, 25.0 ]
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 12.0,
									"frgb" : 0.0,
									"id" : "obj-13",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 81.0, 280.0, 192.0, 20.0 ],
									"text" : "<< autogen keydown and key up"
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 12.0,
									"id" : "obj-10",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 2,
									"outlettype" : [ "", "" ],
									"patching_rect" : [ 6.0, 243.0, 32.5, 20.0 ],
									"text" : "t l l"
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 12.0,
									"id" : "obj-8",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 6.0, 209.0, 27.0, 20.0 ],
									"text" : "iter"
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 12.0,
									"id" : "obj-7",
									"maxclass" : "newobj",
									"numinlets" : 2,
									"numoutlets" : 2,
									"outlettype" : [ "bang", "" ],
									"patching_rect" : [ 65.0, 116.0, 36.0, 20.0 ],
									"text" : "sel 1"
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 12.0,
									"id" : "obj-6",
									"maxclass" : "newobj",
									"numinlets" : 2,
									"numoutlets" : 2,
									"outlettype" : [ "", "" ],
									"patching_rect" : [ 6.0, 162.0, 40.0, 20.0 ],
									"text" : "zl reg"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-5",
									"maxclass" : "button",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "bang" ],
									"patching_rect" : [ 81.0, 162.0, 20.0, 20.0 ]
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 12.0,
									"id" : "obj-3",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 103.0, 237.0, 122.0, 18.0 ],
									"text" : "1 1"
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 12.0,
									"id" : "obj-19",
									"maxclass" : "newobj",
									"numinlets" : 2,
									"numoutlets" : 2,
									"outlettype" : [ "", "" ],
									"patching_rect" : [ 27.0, 80.0, 57.0, 20.0 ],
									"text" : "zl ecils 1"
								}

							}
, 							{
								"box" : 								{
									"comment" : "input macro sequence",
									"id" : "obj-1",
									"maxclass" : "inlet",
									"numinlets" : 0,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 27.0, 23.0, 25.0, 25.0 ]
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"destination" : [ "obj-19", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-1", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-15", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-10", 1 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-16", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-10", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-17", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-15", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-17", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-16", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-6", 1 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-19", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-7", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-19", 1 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-3", 1 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-6", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-5", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-6", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-8", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-6", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-6", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-7", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-10", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-8", 0 ]
								}

							}
 ]
					}
,
					"patching_rect" : [ 251.0, 272.0, 58.0, 21.0 ],
					"saved_object_attributes" : 					{
						"default_fontface" : 0,
						"default_fontname" : "Arial",
						"default_fontsize" : 12.0,
						"description" : "",
						"digest" : "",
						"fontface" : 0,
						"fontname" : "Arial",
						"fontsize" : 12.0,
						"globalpatchername" : "",
						"tags" : ""
					}
,
					"text" : "p macros"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Optima Regular",
					"fontsize" : 12.0,
					"id" : "obj-38",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"patching_rect" : [ 264.0, 238.0, 73.0, 21.0 ],
					"text" : "route macro"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Optima Regular",
					"fontsize" : 12.0,
					"id" : "obj-2",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 395.0, 228.0, 73.0, 19.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 114.0, 122.0, 122.0, 19.0 ]
				}

			}
, 			{
				"box" : 				{
					"coll_data" : 					{
						"count" : 94,
						"data" : [ 							{
								"key" : "A",
								"value" : [ 65 ]
							}
, 							{
								"key" : "B",
								"value" : [ 66 ]
							}
, 							{
								"key" : "C",
								"value" : [ 67 ]
							}
, 							{
								"key" : "D",
								"value" : [ 68 ]
							}
, 							{
								"key" : "E",
								"value" : [ 69 ]
							}
, 							{
								"key" : "F",
								"value" : [ 70 ]
							}
, 							{
								"key" : "G",
								"value" : [ 71 ]
							}
, 							{
								"key" : "H",
								"value" : [ 72 ]
							}
, 							{
								"key" : "I",
								"value" : [ 73 ]
							}
, 							{
								"key" : "J",
								"value" : [ 74 ]
							}
, 							{
								"key" : "K",
								"value" : [ 75 ]
							}
, 							{
								"key" : "L",
								"value" : [ 76 ]
							}
, 							{
								"key" : "M",
								"value" : [ 77 ]
							}
, 							{
								"key" : "N",
								"value" : [ 78 ]
							}
, 							{
								"key" : "O",
								"value" : [ 79 ]
							}
, 							{
								"key" : "P",
								"value" : [ 80 ]
							}
, 							{
								"key" : "Q",
								"value" : [ 81 ]
							}
, 							{
								"key" : "R",
								"value" : [ 82 ]
							}
, 							{
								"key" : "S",
								"value" : [ 83 ]
							}
, 							{
								"key" : "T",
								"value" : [ 84 ]
							}
, 							{
								"key" : "U",
								"value" : [ 85 ]
							}
, 							{
								"key" : "V",
								"value" : [ 86 ]
							}
, 							{
								"key" : "W",
								"value" : [ 87 ]
							}
, 							{
								"key" : "X",
								"value" : [ 88 ]
							}
, 							{
								"key" : "Y",
								"value" : [ 89 ]
							}
, 							{
								"key" : "Z",
								"value" : [ 90 ]
							}
, 							{
								"key" : 0,
								"value" : [ 48 ]
							}
, 							{
								"key" : 1,
								"value" : [ 49 ]
							}
, 							{
								"key" : 2,
								"value" : [ 50 ]
							}
, 							{
								"key" : 3,
								"value" : [ 51 ]
							}
, 							{
								"key" : 4,
								"value" : [ 52 ]
							}
, 							{
								"key" : 5,
								"value" : [ 53 ]
							}
, 							{
								"key" : 6,
								"value" : [ 54 ]
							}
, 							{
								"key" : 7,
								"value" : [ 55 ]
							}
, 							{
								"key" : 8,
								"value" : [ 56 ]
							}
, 							{
								"key" : 9,
								"value" : [ 57 ]
							}
, 							{
								"key" : "NUM_0",
								"value" : [ 96 ]
							}
, 							{
								"key" : "NUM_1",
								"value" : [ 97 ]
							}
, 							{
								"key" : "NUM_2",
								"value" : [ 98 ]
							}
, 							{
								"key" : "NUM_3",
								"value" : [ 99 ]
							}
, 							{
								"key" : "NUM_4",
								"value" : [ 100 ]
							}
, 							{
								"key" : "NUM_5",
								"value" : [ 101 ]
							}
, 							{
								"key" : "NUM_6",
								"value" : [ 102 ]
							}
, 							{
								"key" : "NUM_7",
								"value" : [ 103 ]
							}
, 							{
								"key" : "NUM_8",
								"value" : [ 104 ]
							}
, 							{
								"key" : "NUM_9",
								"value" : [ 105 ]
							}
, 							{
								"key" : "MULT",
								"value" : [ 106 ]
							}
, 							{
								"key" : "ADD",
								"value" : [ 107 ]
							}
, 							{
								"key" : "ENTER",
								"value" : [ 13 ]
							}
, 							{
								"key" : "SUB",
								"value" : [ 109 ]
							}
, 							{
								"key" : "DEC",
								"value" : [ 110 ]
							}
, 							{
								"key" : "DIV",
								"value" : [ 111 ]
							}
, 							{
								"key" : "F1",
								"value" : [ 112 ]
							}
, 							{
								"key" : "F2",
								"value" : [ 113 ]
							}
, 							{
								"key" : "F3",
								"value" : [ 114 ]
							}
, 							{
								"key" : "F4",
								"value" : [ 115 ]
							}
, 							{
								"key" : "F5",
								"value" : [ 116 ]
							}
, 							{
								"key" : "F6",
								"value" : [ 117 ]
							}
, 							{
								"key" : "F7",
								"value" : [ 118 ]
							}
, 							{
								"key" : "F8",
								"value" : [ 119 ]
							}
, 							{
								"key" : "F9",
								"value" : [ 120 ]
							}
, 							{
								"key" : "F11",
								"value" : [ 122 ]
							}
, 							{
								"key" : "F12",
								"value" : [ 123 ]
							}
, 							{
								"key" : "F13",
								"value" : [ 124 ]
							}
, 							{
								"key" : "F14",
								"value" : [ 125 ]
							}
, 							{
								"key" : "F15",
								"value" : [ 126 ]
							}
, 							{
								"key" : "BACKSPACE",
								"value" : [ 8 ]
							}
, 							{
								"key" : "TAB",
								"value" : [ 9 ]
							}
, 							{
								"key" : "SHIFT",
								"value" : [ 16 ]
							}
, 							{
								"key" : "CONTROL",
								"value" : [ 17 ]
							}
, 							{
								"key" : "CAPSLOCK",
								"value" : [ 20 ]
							}
, 							{
								"key" : "ESCAPE",
								"value" : [ 27 ]
							}
, 							{
								"key" : "SPACEBAR",
								"value" : [ 32 ]
							}
, 							{
								"key" : "PAGEUP",
								"value" : [ 33 ]
							}
, 							{
								"key" : "PAGEDOWN",
								"value" : [ 34 ]
							}
, 							{
								"key" : "END",
								"value" : [ 35 ]
							}
, 							{
								"key" : "HOME",
								"value" : [ 36 ]
							}
, 							{
								"key" : "LEFT",
								"value" : [ 37 ]
							}
, 							{
								"key" : "UP",
								"value" : [ 38 ]
							}
, 							{
								"key" : "RIGHT",
								"value" : [ 39 ]
							}
, 							{
								"key" : "DOWN",
								"value" : [ 40 ]
							}
, 							{
								"key" : "DEL",
								"value" : [ 46 ]
							}
, 							{
								"key" : "NUMLOCK",
								"value" : [ 144 ]
							}
, 							{
								"key" : "PAUSE",
								"value" : [ 19 ]
							}
, 							{
								"key" : "SEMICOLON",
								"value" : [ 186 ]
							}
, 							{
								"key" : "=",
								"value" : [ 187 ]
							}
, 							{
								"key" : "-",
								"value" : [ 189 ]
							}
, 							{
								"key" : "FRONTSLASH",
								"value" : [ 191 ]
							}
, 							{
								"key" : "`",
								"value" : [ 192 ]
							}
, 							{
								"key" : "[",
								"value" : [ 219 ]
							}
, 							{
								"key" : "BACKSLASH",
								"value" : [ 220 ]
							}
, 							{
								"key" : "]",
								"value" : [ 221 ]
							}
, 							{
								"key" : "BACKAPOSTROPHE",
								"value" : [ 188 ]
							}
, 							{
								"key" : ".",
								"value" : [ 190 ]
							}
 ]
					}
,
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"id" : "obj-20",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 4,
					"outlettype" : [ "", "", "", "" ],
					"patching_rect" : [ 351.0, 426.0, 138.0, 20.0 ],
					"saved_object_attributes" : 					{
						"embed" : 1
					}
,
					"text" : "coll convert_to_key_PC"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Optima Regular",
					"fontsize" : 12.0,
					"id" : "obj-48",
					"maxclass" : "newobj",
					"numinlets" : 3,
					"numoutlets" : 3,
					"outlettype" : [ "", "", "" ],
					"patching_rect" : [ 264.0, 334.5, 93.0, 21.0 ],
					"text" : "route MB1 MB2"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Impact",
					"fontsize" : 14.0,
					"frgb" : 0.0,
					"id" : "obj-33",
					"linecount" : 2,
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 554.0, 228.0, 115.0, 41.0 ],
					"text" : "< MAC\n< PC",
					"textcolor" : [ 0.964065, 0.988959, 1.0, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Impact",
					"fontsize" : 14.0,
					"frgb" : 0.0,
					"id" : "obj-4",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 513.0, 508.0, 193.0, 24.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 34.0, 59.0, 193.0, 24.0 ],
					"text" : "Turn mouse on/off (space bar)",
					"textcolor" : [ 0.964065, 0.988959, 1.0, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"id" : "obj-34",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 610.0, 456.0, 72.0, 20.0 ],
					"text" : "loadmess 0"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"id" : "obj-29",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 2,
					"outlettype" : [ "bang", "" ],
					"patching_rect" : [ 692.5, 479.0, 43.0, 20.0 ],
					"text" : "sel 32"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"id" : "obj-28",
					"maxclass" : "newobj",
					"numinlets" : 0,
					"numoutlets" : 4,
					"outlettype" : [ "int", "int", "int", "int" ],
					"patching_rect" : [ 692.5, 456.0, 59.5, 20.0 ],
					"text" : "key"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-26",
					"maxclass" : "toggle",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "int" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 692.5, 508.0, 20.0, 20.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 10.0, 59.0, 20.0, 20.0 ]
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"id" : "obj-5",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "bang" ],
					"patching_rect" : [ 692.5, 539.0, 51.0, 20.0 ],
					"text" : "metro 2"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"id" : "obj-13",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"patching_rect" : [ 318.0, 504.5, 39.0, 20.0 ],
					"text" : "zl rev"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-72",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "bang" ],
					"patching_rect" : [ 553.0, 390.5, 45.0, 17.0 ],
					"text" : "del 3000"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-15",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "bang" ],
					"patching_rect" : [ 553.0, 369.5, 48.0, 17.0 ],
					"text" : "loadbang"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-51",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"patching_rect" : [ 553.0, 440.5, 48.0, 17.0 ],
					"text" : "route text"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 14.0,
					"frgb" : 0.0,
					"id" : "obj-16",
					"keymode" : 1,
					"maxclass" : "textedit",
					"numinlets" : 1,
					"numoutlets" : 4,
					"outlettype" : [ "", "int", "", "" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 553.0, 415.5, 36.0, 17.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 8.0, 84.5, 36.0, 20.0 ],
					"text" : "8"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"frgb" : 0.0,
					"id" : "obj-91",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 660.5, 371.0, 115.0, 20.0 ],
					"text" : "make 'at rest zone'"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-90",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 877.5, 389.5, 170.0, 17.0 ],
					"text" : "if $f1 < 0.1 && $f1 > -0.1 then 0 else $f1"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-86",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 775.5, 371.0, 170.0, 17.0 ],
					"text" : "if $f1 < 0.1 && $f1 > -0.1 then 0 else $f1"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-85",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "float" ],
					"patching_rect" : [ 877.5, 426.0, 32.5, 17.0 ],
					"text" : "* 1."
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Impact",
					"fontsize" : 14.0,
					"frgb" : 0.0,
					"id" : "obj-84",
					"linecount" : 2,
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 523.0, 415.5, 108.0, 41.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 40.0, 84.5, 195.0, 24.0 ],
					"text" : "Look sensitivity, Higher is faster",
					"textcolor" : [ 0.964065, 0.988959, 1.0, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-82",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "float" ],
					"patching_rect" : [ 792.0, 426.0, 32.5, 17.0 ],
					"text" : "* 1."
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"id" : "obj-77",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "int", "int" ],
					"patching_rect" : [ 1131.0, 225.0, 32.5, 20.0 ],
					"text" : "t i 1"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"id" : "obj-76",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "int", "int" ],
					"patching_rect" : [ 966.0, 225.0, 32.5, 20.0 ],
					"text" : "t i 0"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-71",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 792.0, 476.0, 45.0, 17.0 ],
					"text" : "pak 0. 0."
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"id" : "obj-68",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"patching_rect" : [ 1023.0, 335.0, 41.0, 20.0 ],
					"text" : "zl join"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-61",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 1023.0, 287.0, 59.0, 15.0 ],
					"text" : "buttonDown"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-62",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 966.0, 287.0, 47.0, 15.0 ],
					"text" : "buttonUp"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-17",
					"maxclass" : "newobj",
					"numinlets" : 3,
					"numoutlets" : 3,
					"outlettype" : [ "bang", "bang", "" ],
					"patching_rect" : [ 966.0, 262.0, 133.0, 17.0 ],
					"text" : "sel 0 1"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-64",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 792.0, 508.0, 64.0, 15.0 ],
					"text" : "move $1 $2"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"id" : "obj-60",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 902.0, 548.0, 81.0, 20.0 ],
					"text" : "print MOUSE"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"id" : "obj-52",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patcher" : 					{
						"fileversion" : 1,
						"appversion" : 						{
							"major" : 6,
							"minor" : 1,
							"revision" : 8,
							"architecture" : "x86"
						}
,
						"rect" : [ 41.0, 189.0, 610.0, 272.0 ],
						"bglocked" : 0,
						"openinpresentation" : 0,
						"default_fontsize" : 12.0,
						"default_fontface" : 0,
						"default_fontname" : "Arial",
						"gridonopen" : 0,
						"gridsize" : [ 15.0, 15.0 ],
						"gridsnaponopen" : 0,
						"statusbarvisible" : 2,
						"toolbarvisible" : 1,
						"boxanimatetime" : 200,
						"imprint" : 0,
						"enablehscroll" : 1,
						"enablevscroll" : 1,
						"devicewidth" : 0.0,
						"description" : "",
						"digest" : "",
						"tags" : "",
						"boxes" : [ 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 12.0,
									"frgb" : 0.0,
									"id" : "obj-16",
									"linecount" : 2,
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 63.5, 72.5, 150.0, 33.0 ],
									"text" : "<< bypass Uppercase if data is INT"
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 12.0,
									"id" : "obj-14",
									"maxclass" : "newobj",
									"numinlets" : 2,
									"numoutlets" : 2,
									"outlettype" : [ "", "" ],
									"patching_rect" : [ 8.0, 79.0, 54.0, 20.0 ],
									"text" : "route int"
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 12.0,
									"frgb" : 0.0,
									"id" : "obj-6",
									"linecount" : 2,
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 185.0, 10.0, 454.0, 34.0 ],
									"text" : "Thanks to Charles on this post:\nhttp://cycling74.com/forums/topic/textstring-lowerupper-case-change/"
								}

							}
, 							{
								"box" : 								{
									"comment" : "ALL UPPERCASE",
									"id" : "obj-3",
									"maxclass" : "outlet",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 43.0, 199.0, 25.0, 25.0 ]
								}

							}
, 							{
								"box" : 								{
									"comment" : "Ensure all Uppercase",
									"id" : "obj-1",
									"maxclass" : "inlet",
									"numinlets" : 0,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 43.0, 31.0, 25.0, 25.0 ]
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 10.0,
									"id" : "obj-13",
									"maxclass" : "newobj",
									"numinlets" : 3,
									"numoutlets" : 1,
									"outlettype" : [ "list" ],
									"patching_rect" : [ 43.0, 111.0, 46.0, 18.0 ],
									"text" : "atoi"
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 12.0,
									"id" : "obj-29",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 43.0, 135.0, 191.0, 20.0 ],
									"text" : "vexpr $i1-32*($i1>96 && $i1<123)"
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 12.0,
									"id" : "obj-4",
									"maxclass" : "newobj",
									"numinlets" : 3,
									"numoutlets" : 1,
									"outlettype" : [ "int" ],
									"patching_rect" : [ 43.0, 160.0, 46.0, 20.0 ],
									"text" : "itoa"
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"destination" : [ "obj-14", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-1", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-29", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-13", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-13", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-14", 1 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-3", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"midpoints" : [ 17.5, 191.0, 52.5, 191.0 ],
									"source" : [ "obj-14", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-4", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-29", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-3", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-4", 0 ]
								}

							}
 ]
					}
,
					"patching_rect" : [ 318.0, 303.0, 111.0, 20.0 ],
					"saved_object_attributes" : 					{
						"default_fontface" : 0,
						"default_fontname" : "Arial",
						"default_fontsize" : 12.0,
						"description" : "",
						"digest" : "",
						"fontface" : 0,
						"fontname" : "Arial",
						"fontsize" : 12.0,
						"globalpatchername" : "",
						"tags" : ""
					}
,
					"text" : "p to_UPPERCASE"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"id" : "obj-22",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 383.0, 531.5, 104.0, 20.0 ],
					"text" : "print KEYBOARD"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"id" : "obj-32",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"patching_rect" : [ 318.0, 479.0, 41.0, 20.0 ],
					"text" : "zl join"
				}

			}
, 			{
				"box" : 				{
					"coll_data" : 					{
						"count" : 62,
						"data" : [ 							{
								"key" : "A",
								"value" : [ 0 ]
							}
, 							{
								"key" : "S",
								"value" : [ 1 ]
							}
, 							{
								"key" : "D",
								"value" : [ 2 ]
							}
, 							{
								"key" : "F",
								"value" : [ 3 ]
							}
, 							{
								"key" : "H",
								"value" : [ 4 ]
							}
, 							{
								"key" : "G",
								"value" : [ 5 ]
							}
, 							{
								"key" : "Z",
								"value" : [ 6 ]
							}
, 							{
								"key" : "X",
								"value" : [ 7 ]
							}
, 							{
								"key" : "C",
								"value" : [ 8 ]
							}
, 							{
								"key" : "V",
								"value" : [ 9 ]
							}
, 							{
								"key" : "B",
								"value" : [ 11 ]
							}
, 							{
								"key" : "Q",
								"value" : [ 12 ]
							}
, 							{
								"key" : "W",
								"value" : [ 13 ]
							}
, 							{
								"key" : "E",
								"value" : [ 14 ]
							}
, 							{
								"key" : "R",
								"value" : [ 15 ]
							}
, 							{
								"key" : "Y",
								"value" : [ 16 ]
							}
, 							{
								"key" : "T",
								"value" : [ 17 ]
							}
, 							{
								"key" : 1,
								"value" : [ 18 ]
							}
, 							{
								"key" : 2,
								"value" : [ 19 ]
							}
, 							{
								"key" : 3,
								"value" : [ 20 ]
							}
, 							{
								"key" : 4,
								"value" : [ 21 ]
							}
, 							{
								"key" : 6,
								"value" : [ 22 ]
							}
, 							{
								"key" : 5,
								"value" : [ 23 ]
							}
, 							{
								"key" : "=",
								"value" : [ 24 ]
							}
, 							{
								"key" : 9,
								"value" : [ 25 ]
							}
, 							{
								"key" : 7,
								"value" : [ 26 ]
							}
, 							{
								"key" : "-",
								"value" : [ 27 ]
							}
, 							{
								"key" : 8,
								"value" : [ 28 ]
							}
, 							{
								"key" : 0,
								"value" : [ 29 ]
							}
, 							{
								"key" : "]",
								"value" : [ 30 ]
							}
, 							{
								"key" : "O",
								"value" : [ 31 ]
							}
, 							{
								"key" : "U",
								"value" : [ 32 ]
							}
, 							{
								"key" : "[",
								"value" : [ 33 ]
							}
, 							{
								"key" : "I",
								"value" : [ 34 ]
							}
, 							{
								"key" : "P",
								"value" : [ 35 ]
							}
, 							{
								"key" : "RETURN",
								"value" : [ 36 ]
							}
, 							{
								"key" : "ENTER",
								"value" : [ 36 ]
							}
, 							{
								"key" : "L",
								"value" : [ 37 ]
							}
, 							{
								"key" : "J",
								"value" : [ 38 ]
							}
, 							{
								"key" : "'",
								"value" : [ 39 ]
							}
, 							{
								"key" : "K",
								"value" : [ 40 ]
							}
, 							{
								"key" : "SEMICOLON",
								"value" : [ 41 ]
							}
, 							{
								"key" : "FRONTSLASH",
								"value" : [ 42 ]
							}
, 							{
								"key" : "COMMA",
								"value" : [ 43 ]
							}
, 							{
								"key" : "BACKSLASH",
								"value" : [ 44 ]
							}
, 							{
								"key" : "N",
								"value" : [ 45 ]
							}
, 							{
								"key" : "M",
								"value" : [ 46 ]
							}
, 							{
								"key" : ".",
								"value" : [ 47 ]
							}
, 							{
								"key" : "TAB",
								"value" : [ 48 ]
							}
, 							{
								"key" : "SPACE",
								"value" : [ 49 ]
							}
, 							{
								"key" : "BACKAPOSTROPHE",
								"value" : [ 50 ]
							}
, 							{
								"key" : "DELETE",
								"value" : [ 51 ]
							}
, 							{
								"key" : "ESCAPE",
								"value" : [ 53 ]
							}
, 							{
								"key" : "COMMAND",
								"value" : [ 55 ]
							}
, 							{
								"key" : "SHIFT",
								"value" : [ 56 ]
							}
, 							{
								"key" : "CAPSLOCK",
								"value" : [ 57 ]
							}
, 							{
								"key" : "OPTION",
								"value" : [ 58 ]
							}
, 							{
								"key" : "CONTROL",
								"value" : [ 59 ]
							}
, 							{
								"key" : "UP",
								"value" : [ 126 ]
							}
, 							{
								"key" : "DOWN",
								"value" : [ 125 ]
							}
, 							{
								"key" : "LEFT",
								"value" : [ 123 ]
							}
, 							{
								"key" : "RIGHT",
								"value" : [ 124 ]
							}
 ]
					}
,
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"id" : "obj-37",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 4,
					"outlettype" : [ "", "", "", "" ],
					"patching_rect" : [ 318.0, 400.5, 143.0, 20.0 ],
					"saved_object_attributes" : 					{
						"embed" : 1
					}
,
					"text" : "coll convert_to_key_mac"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-53",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 432.0, 367.5, 35.0, 15.0 ],
					"text" : "keyup"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-65",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 383.0, 367.5, 46.0, 15.0 ],
					"text" : "keydown"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"id" : "obj-66",
					"maxclass" : "newobj",
					"numinlets" : 3,
					"numoutlets" : 3,
					"outlettype" : [ "bang", "bang", "" ],
					"patching_rect" : [ 383.0, 334.5, 46.0, 20.0 ],
					"text" : "sel 1 0"
				}

			}
, 			{
				"box" : 				{
					"color" : [ 1.0, 0.360784, 0.682353, 1.0 ],
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-67",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 840.5, 549.5, 55.0, 17.0 ],
					"text" : "aka.mouse",
					"varname" : "mouse_controller[1]"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"frgb" : 0.0,
					"id" : "obj-69",
					"linecount" : 2,
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 429.0, 277.5, 354.0, 34.0 ],
					"text" : "<< Get 1 or 0 button data event from stream out of Right outlet.\nKey message converted to key code from left outled"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"id" : "obj-70",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"patching_rect" : [ 318.0, 272.0, 57.0, 20.0 ],
					"text" : "zl ecils 1"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"id" : "obj-73",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 1131.0, 195.0, 107.0, 20.0 ],
					"text" : "receiverOSC MB2"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"id" : "obj-74",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 966.0, 195.0, 107.0, 20.0 ],
					"text" : "receiverOSC MB1"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"id" : "obj-75",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"patching_rect" : [ 775.5, 345.0, 59.0, 20.0 ],
					"text" : "route MX"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"id" : "obj-78",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"patching_rect" : [ 877.5, 345.0, 59.0, 20.0 ],
					"text" : "route MY"
				}

			}
, 			{
				"box" : 				{
					"color" : [ 1.0, 0.360784, 0.682353, 1.0 ],
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"id" : "obj-80",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 318.0, 576.0, 69.0, 17.0 ],
					"text" : "aka.keyboard",
					"varname" : "mouse_controller"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"id" : "obj-81",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"patching_rect" : [ 264.0, 200.0, 65.0, 20.0 ],
					"text" : "route KEY"
				}

			}
, 			{
				"box" : 				{
					"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"frgb" : 0.0,
					"id" : "obj-59",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 175.0, 5.0, 94.0, 17.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 7.0, 33.5, 94.0, 17.0 ],
					"text" : "Input keywords 1/0"
				}

			}
, 			{
				"box" : 				{
					"comment" : "Datastream to MIDI",
					"id" : "obj-58",
					"maxclass" : "inlet",
					"numinlets" : 0,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 71.0, 35.0, 25.0, 25.0 ]
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 9.0,
					"frgb" : 0.0,
					"id" : "obj-24",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 38.0, 193.0, 91.0, 17.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 26.0, 123.0, 89.0, 17.0 ],
					"text" : "Switchblade Output"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"id" : "obj-14",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 867.0, 68.5, 37.0, 18.0 ],
					"text" : "open"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"id" : "obj-12",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 839.5, 99.0, 53.0, 20.0 ],
					"text" : "pcontrol"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"id" : "obj-11",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patcher" : 					{
						"fileversion" : 1,
						"appversion" : 						{
							"major" : 6,
							"minor" : 1,
							"revision" : 8,
							"architecture" : "x86"
						}
,
						"rect" : [ 25.0, 69.0, 443.0, 167.0 ],
						"bglocked" : 0,
						"openinpresentation" : 0,
						"default_fontsize" : 12.0,
						"default_fontface" : 0,
						"default_fontname" : "Arial",
						"gridonopen" : 0,
						"gridsize" : [ 15.0, 15.0 ],
						"gridsnaponopen" : 0,
						"statusbarvisible" : 2,
						"toolbarvisible" : 1,
						"boxanimatetime" : 200,
						"imprint" : 0,
						"enablehscroll" : 1,
						"enablevscroll" : 1,
						"devicewidth" : 0.0,
						"description" : "",
						"digest" : "",
						"tags" : "",
						"boxes" : [ 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 12.0,
									"frgb" : 0.0,
									"id" : "obj-13",
									"linecount" : 10,
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 222.0, 160.0, 150.0, 141.0 ],
									"text" : "hint: use the word: macro, to \nsend a sequence of key\nstrokes.\nexample:\nmarco A B C ENTER\nwill type \"abc\" and make\nthen press enter.\nMB1 = mousebutton1\nMB2 = mousebutton2"
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 12.0,
									"frgb" : 0.0,
									"id" : "obj-4",
									"linecount" : 65,
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 9.0, 160.0, 167.0, 879.0 ],
									"text" : "Key names:\nA\rS\rD\rF\rH\rG\rZ\rX\rC\rV\rB\rQ\rW\rE\rR\rY\rT\r1\r2\r3\r4\r6\r5\r=\r9\r7\r-\r8\r0\r]\rO\rU\r[\rI\rP\rRETURN\rENTER\rL\rJ\r'\rK\rSEMICOLON\rFRONTSLASH\rCOMMA\rBACKSLASH\rN\rM\r.\rTAB\nSPACE\rBACKAPOSTROPHE\rDELETE\rESCAPE\rCOMMAND\rSHIFT\rCAPSLOCK\rOPTION\rCONTROL\rUP\rDOWN\rLEFT\rRIGHT\nMB1\nMB2"
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 12.0,
									"frgb" : 0.0,
									"id" : "obj-43",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 9.0, 53.0, 342.0, 20.0 ],
									"presentation" : 1,
									"presentation_rect" : [ 260.5, 19.0, 356.0, 20.0 ],
									"text" : "** FIRST MESSAGE MUST be one of the following keywords:"
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 12.0,
									"frgb" : 0.0,
									"id" : "obj-40",
									"linecount" : 5,
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 9.0, 76.0, 174.0, 74.0 ],
									"presentation" : 1,
									"presentation_linecount" : 5,
									"presentation_rect" : [ 260.5, 41.5, 191.0, 74.0 ],
									"text" : "1) KEY - key pad data\n2) MX - mouse x axis\n3) MY - mouse y axis\n4) MB1 - mouse button 1 click\n5) MB2 - mouse button 2 click"
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 12.0,
									"hidden" : 1,
									"id" : "obj-11",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 484.5, 82.0, 47.0, 18.0 ],
									"text" : "wclose"
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 12.0,
									"id" : "obj-9",
									"maxclass" : "textbutton",
									"numinlets" : 1,
									"numoutlets" : 3,
									"outlettype" : [ "", "", "int" ],
									"parameter_enable" : 0,
									"patching_rect" : [ 248.5, 91.0, 54.0, 30.0 ],
									"presentation" : 1,
									"presentation_rect" : [ 340.0, 33.0, 25.0, 26.0 ],
									"text" : "X"
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 12.0,
									"hidden" : 1,
									"id" : "obj-7",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 2,
									"outlettype" : [ "", "" ],
									"patching_rect" : [ 484.5, 139.0, 69.0, 20.0 ],
									"save" : [ "#N", "thispatcher", ";", "#Q", "end", ";" ],
									"text" : "thispatcher"
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 12.0,
									"frgb" : 0.0,
									"id" : "obj-3",
									"linecount" : 2,
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 9.0, 16.0, 375.0, 33.0 ],
									"text" : "Switchblade Module: emulates keyboard and mouse to interact with computer programs! Input keywords followed by threshold 1 / 0 data"
								}

							}
, 							{
								"box" : 								{
									"comment" : "",
									"hidden" : 1,
									"id" : "obj-1",
									"maxclass" : "inlet",
									"numinlets" : 0,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 425.0, 7.0, 25.0, 25.0 ]
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"destination" : [ "obj-7", 0 ],
									"disabled" : 0,
									"hidden" : 1,
									"source" : [ "obj-11", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-11", 0 ],
									"disabled" : 0,
									"hidden" : 1,
									"source" : [ "obj-9", 0 ]
								}

							}
 ]
					}
,
					"patching_rect" : [ 839.5, 126.0, 43.0, 20.0 ],
					"saved_object_attributes" : 					{
						"default_fontface" : 0,
						"default_fontname" : "Arial",
						"default_fontsize" : 12.0,
						"description" : "",
						"digest" : "",
						"fontface" : 0,
						"fontname" : "Arial",
						"fontsize" : 12.0,
						"globalpatchername" : "",
						"tags" : ""
					}
,
					"text" : "p help"
				}

			}
, 			{
				"box" : 				{
					"bubble" : 1,
					"bubbleside" : 3,
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"frgb" : 0.0,
					"id" : "obj-10",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 739.5, 63.0, 100.0, 24.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 140.0, 4.0, 55.0, 24.0 ],
					"text" : "Help"
				}

			}
, 			{
				"box" : 				{
					"fgcolor" : [ 0.339343, 0.458428, 1.0, 1.0 ],
					"id" : "obj-9",
					"maxclass" : "button",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "bang" ],
					"patching_rect" : [ 839.5, 65.0, 20.0, 20.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 194.0, 4.0, 20.0, 20.0 ]
				}

			}
, 			{
				"box" : 				{
					"autofit" : 1,
					"id" : "obj-3",
					"maxclass" : "fpic",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 2.0, 186.0, 36.0, 34.0 ],
					"pic" : "output.png",
					"presentation" : 1,
					"presentation_rect" : [ 1.0, 119.0, 25.0, 25.0 ]
				}

			}
, 			{
				"box" : 				{
					"autofit" : 1,
					"id" : "obj-1",
					"maxclass" : "fpic",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 2.0, 5.0, 36.0, 34.0 ],
					"pic" : "input.png",
					"presentation" : 1,
					"presentation_rect" : [ 1.0, 4.0, 25.0, 25.0 ]
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"frgb" : 0.0,
					"id" : "obj-6",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 38.0, 5.0, 113.0, 20.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 26.0, 4.0, 113.0, 20.0 ],
					"text" : "Switchblade"
				}

			}
, 			{
				"box" : 				{
					"border" : 1,
					"grad1" : [ 0.537255, 0.537255, 0.537255, 1.0 ],
					"grad2" : [ 0.258824, 0.258824, 0.258824, 1.0 ],
					"id" : "obj-31",
					"maxclass" : "panel",
					"mode" : 1,
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 867.0, 15.0, 33.5, 24.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 5.0, 26.5, 225.0, 91.0 ]
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"destination" : [ "obj-11", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-12", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-22", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-13", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-80", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-13", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-12", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-14", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-72", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-15", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-51", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-16", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-61", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-17", 1 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-62", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-17", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-32", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-20", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-5", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-26", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-29", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-28", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-26", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-29", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-13", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-32", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-26", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-34", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-32", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-37", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-2", 1 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-38", 1 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-40", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-38", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-70", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-38", 1 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-70", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-40", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-20", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-44", 1 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-37", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-44", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-46", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-45", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-47", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-45", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-44", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-46", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-49", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-47", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-44", 1 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-48", 2 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-76", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-48", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-77", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-48", 1 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-45", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-49", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-64", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-5", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-54", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-50", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-82", 1 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-51", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-85", 1 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-51", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-48", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-52", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-32", 1 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-53", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-47", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-54", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-73", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-58", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-74", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-58", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-75", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-58", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-78", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-58", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-81", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-58", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-68", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-61", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-68", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-62", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-60", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-64", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-67", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-64", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-32", 1 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-65", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-53", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-66", 1 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-65", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-66", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-60", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-68", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-67", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-68", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-42", 1 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-70", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-52", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-70", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-66", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-70", 1 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-64", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-71", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-16", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-72", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-77", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-73", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-76", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-74", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-86", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-75", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-17", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-76", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-68", 1 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-76", 1 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-17", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-77", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-68", 1 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-77", 1 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-90", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-78", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-38", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-81", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-71", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-82", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-71", 1 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-85", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-82", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-86", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-14", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-9", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-85", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-90", 0 ]
				}

			}
 ],
		"dependency_cache" : [ 			{
				"name" : "input.png",
				"bootpath" : "/Applications/Max6.1/Cycling '74/DataProcessorPlugins",
				"patcherrelativepath" : ".",
				"type" : "PNG",
				"implicit" : 1
			}
, 			{
				"name" : "output.png",
				"bootpath" : "/Applications/Max6.1/Cycling '74/DataProcessorPlugins",
				"patcherrelativepath" : ".",
				"type" : "PNG ",
				"implicit" : 1
			}
, 			{
				"name" : "receiverOSC.maxpat",
				"bootpath" : "/Applications/Max6.1/Cycling '74/eMotionV2.8",
				"patcherrelativepath" : "../eMotionV2.8",
				"type" : "JSON",
				"implicit" : 1
			}
, 			{
				"name" : "aka.keyboard.mxo",
				"type" : "iLaX"
			}
, 			{
				"name" : "aka.mouse.mxo",
				"type" : "iLaX"
			}
 ]
	}

}
