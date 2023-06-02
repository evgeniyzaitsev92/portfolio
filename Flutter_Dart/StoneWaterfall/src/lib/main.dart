// ignore_for_file: avoid_print, prefer_const_constructors, prefer_const_literals_to_create_immutables, sort_child_properties_last, no_logic_in_create_state, constant_identifier_names, invalid_use_of_visible_for_testing_member
import 'dart:async';
import 'package:flutter/material.dart';
import 'package:audioplayers/audioplayers.dart';
import 'package:shared_preferences/shared_preferences.dart';

void main() async {
  runApp(const MyApp());
}

const Color CLR_DEF_BLUE = Color.fromRGBO(115, 115, 255, 1);
const Color CLR_PRESSED = Color.fromRGBO(0, 200, 0, 1);

class MyApp extends StatelessWidget {
  const MyApp({super.key});

  // This widget is the root of your application.
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Waterfall',
      theme: ThemeData.dark(),
      home: const Waterfall(title: 'Waterfall'),
    );
  }
}

class ButtonChangeTime extends StatefulWidget {
  const ButtonChangeTime({super.key, required this.icon, required this.funcDown, required this.funcUp});
  final IconData icon;
  final Function funcDown;
  final Function funcUp;
  
  @override
  State<ButtonChangeTime> createState() => _ButtonChangeTime();
}
// **********************************//
// *** BUTTON FOR CHANGING TIME *** //
class _ButtonChangeTime extends State<ButtonChangeTime> {
  bool buttonPressed = false;
  @override
  Widget build(BuildContext context) {
    return Expanded(
      flex: 1,
      child: Container(
        padding: EdgeInsets.all(1),
        decoration: BoxDecoration(
          color: CLR_DEF_BLUE,
          borderRadius: BorderRadius.all(Radius.circular(9)),
        ),
        child: Container(
          decoration: BoxDecoration(
            color: buttonPressed ? CLR_PRESSED : CLR_DEF_BLUE,
            border: Border.all(color: Color.fromRGBO(50, 50, 50, 1), width: 2),
            borderRadius: BorderRadius.all(Radius.circular(8)),
          ),
          child: Listener(
            onPointerDown: (details) {
              setState(() {buttonPressed = true;});
              widget.funcDown();
            },
            onPointerUp: (details) {
              setState(() {buttonPressed = false;});
              widget.funcUp();
            },
            child: Container(
              decoration: BoxDecoration(),
              child: Icon(
                widget.icon,
                color: Colors.white,
                size: 100.0,
              ),
            ),
          ),
        ),
      ), 
    ); 
  }
}

// *********************//
// *** MAIN WIDGET *** //
class Waterfall extends StatefulWidget {
  const Waterfall({super.key, required this.title});
  final String title;
  @override
  State<Waterfall> createState() => _WaterfallState();
}

class _WaterfallState extends State<Waterfall> {
  // ** VARIABLES ** //
  final player1 = AudioPlayer();
  final player2 = AudioPlayer();
  int countHours = 0;
  int countMinutes = 0;
  double paddingMain = 10;
  double curPeriodSliderValue = 0.0;
  bool isStarted = false;
  bool isStopped = false;
  bool allowPress = true;
  bool buttonPressed = false;
  bool loopActive = false;
  Color? clrBtnStartPause;
  late DateTime timeStart;
  late List<bool> selectedType = <bool>[true, false];
  List periods = [
    [0, 10, "0:10"],
    [0, 20, "0:20"],
    [0, 30, "0:30"],
    [0, 40, "0:40"],
    [0, 50, "0:50"],
    [1, 0, "1:00"],
    [1, 10, "1:10"],
    [1, 20, "1:20"],
    [1, 30, "1:30"],
    [1, 40, "1:40"],
    [1, 50, "1:50"],
    [2, 0, "2:00"],
    [2, 10, "2:10"],
    [2, 20, "2:20"],
    [2, 30, "2:30"],
    [2, 40, "2:40"],
    [2, 50, "2:50"],
    [3, 0, "3:00"],
  ];
  
  // ** FUNCTIONS ** //
  @override
    void initState() {
    super.initState();
    // SharedPreferences.setMockInitialValues({});
    loadPrefs();
  }

  // * Save data * //
  void saveHours() async {
    SharedPreferences prefs = await SharedPreferences.getInstance();
    prefs.setInt("countHours", countHours);
  }

  void saveMinutes() async {
    SharedPreferences prefs = await SharedPreferences.getInstance();
    prefs.setInt("countMinutes", countMinutes);
  }

  void saveSelectedType() async {
    SharedPreferences prefs = await SharedPreferences.getInstance();
    setState(() {
      prefs.setStringList(
        "selectedType",
        selectedType.map((e) => e ? "true" : "false").toList(),
      );
      // prefs.setInt('currentFontFamily', _currentFontFamily);
    });
  }

  void savePeriod() async {
    SharedPreferences prefs = await SharedPreferences.getInstance();
    prefs.setDouble("curPeriodSliderValue", curPeriodSliderValue);
  }

  // * Get data * //
  void getHours() async {
    SharedPreferences prefs = await SharedPreferences.getInstance();
    setState(() {
      countHours = prefs.getInt("countHours") ?? 0;
    });
  }

  void getMinutes() async {
    SharedPreferences prefs = await SharedPreferences.getInstance();
    setState(() {
      countMinutes = prefs.getInt("countMinutes") ?? 0;
    });
  }

  void getSelectedType() async {
    SharedPreferences prefs = await SharedPreferences.getInstance();
    setState(() {
      selectedType = (prefs.getStringList('selectedType')?.map((e) => e == 'true' ? true : false)
              .toList() ?? [true, false]);
    });
  }

  void getPeriod() async {
    SharedPreferences prefs = await SharedPreferences.getInstance();
    setState(() {
      curPeriodSliderValue = prefs.getDouble("curPeriodSliderValue") ?? 0.0;
    });
  }

  void loadPrefs() {
    getHours();
    getMinutes();
    getSelectedType();
    getPeriod();
  }

  // * Change time * //
  void changeTime(String type, String action) async {
    if (loopActive) return;// check if loop is active
    loopActive = true;
    while (buttonPressed){
      if(type == "hours"){
        if(action == "+"){
          setState(() {
            countHours++;
            if(countHours > 23){countHours = 0;}
          });
        } else if(action == "-"){
          setState(() {
            countHours--;
            if(countHours < 0){countHours = 23;}
          });
        }
      } else if(type == "minutes"){
        if(action == "+"){
          setState(() {
            countMinutes++;
            if(countMinutes > 59){countMinutes = 0;}
          });
        } else if(action == "-"){
          setState(() {
            countMinutes--;
            if(countMinutes < 0){countMinutes = 59;}
          });
        }
      }
      
      await Future.delayed(Duration(milliseconds: 125));
    }
    loopActive = false;
  }

  // * Start/pause sound * //
  Future startPause() async {
    // if(allowPress == false){
    //   return;
    // }
    // allowPress = false;
    if(isStarted == false){
      timeStart = DateTime.now();
      setState(() {isStarted = true;});
      if(isStopped == false){
        await playSound();
      }      
    } else {
      setState(() {isStarted = false;});
    }
  }

  // * Play sound * //
  Future playSound() async {
    // bool enableAllowPress = false;
    player1.setReleaseMode(ReleaseMode.loop);
    player2.setReleaseMode(ReleaseMode.loop);
    player1.setPlayerMode(PlayerMode.lowLatency);
    player2.setPlayerMode(PlayerMode.lowLatency);

    int sec = 0;
    while(true){
      if(sec == 0){
        sec++;
        await player1.play(AssetSource("waterfall_10m.mp3"));
        await Future.delayed(Duration(milliseconds: 2000));
        await player2.play(AssetSource("waterfall_10m.mp3"));
      }
      if(isStarted == false){ await stopSound(); break; }
      // if(sec == 300){ sec = 0; }
      if(selectedType[0]){
        if(DateTime.now().compareTo(timeStart.add(Duration(hours: periods[curPeriodSliderValue.toInt()][0], minutes: periods[curPeriodSliderValue.toInt()][1]))) > 0){
          await startPause();
        }
      } else if(selectedType[1]){
        if(DateTime.now().compareTo(timeStart.copyWith(hour: countHours, minute: countMinutes, second: 0, millisecond: 0, microsecond: 0)) > 0){
          await startPause();
        }
      }
      
      // sec++;
      await Future.delayed(Duration(milliseconds: 1000));

      // if(enableAllowPress == false){
      //   await Future.delayed(Duration(milliseconds: 500));
      //   enableAllowPress = true;
      //   allowPress = true;
      // }      
    }
  }

  // * Stop sound * //
  Future stopSound() async {
    isStopped = true;
    int count = 0;
    double volume = 1.0;

    while(count != 2000){
      if(isStarted == true){
        break;
      }
      volume -= 0.0005;
      player1.setVolume(volume);
      player2.setVolume(volume);
      count++;
      await Future.delayed(Duration(microseconds: 500));
    }

    player1.stop();
    player2.stop();
    player1.setVolume(1.0);
    player2.setVolume(1.0);
    isStopped = false;
    if(isStarted == true){
        await playSound();
    }
    // allowPress = true;
  }

  // ** BUILD MAIN WIDGET  ** //
  @override
  Widget build(BuildContext context) {
    return Scaffold(
      // * Main * //
      body: Center(        
        child: Column(
          verticalDirection: VerticalDirection.down,
          children: [
            SizedBox(height: MediaQuery.of(context).viewPadding.top), // screen top status bar
            SizedBox(height:paddingMain), // separator
            // * Period section * //
            Expanded(
              flex:1,
              child: Container(
                margin: EdgeInsets.symmetric(horizontal: paddingMain),
                padding: EdgeInsets.all(paddingMain/2),
                decoration: BoxDecoration(
                  color: Color.fromRGBO(80, 80, 80, 1),
                  border: Border.all(color: selectedType[0] ? CLR_PRESSED : Colors.transparent, width: 5),
                  borderRadius: BorderRadius.all(Radius.circular(8)),
                ),
                child: Container(
                  margin: EdgeInsets.all(paddingMain/2),
                  padding: EdgeInsets.all(paddingMain),
                  decoration: BoxDecoration(
                    color: Color.fromRGBO(30, 30, 30, 1),
                    borderRadius: BorderRadius.all(Radius.circular(8)),
                  ),
                  child: Row(
                    textDirection: TextDirection.ltr,
                    children: <Widget>[

                      // * Period icon * //
                      Expanded(
                        flex: 1,
                        child: Icon(
                          Icons.timer_outlined,
                          color: selectedType[0] ? CLR_PRESSED : Colors.white,
                          size: 50.0,
                        ),
                      ),

                      // * Period slider * //
                      Expanded(
                        flex: 6,
                        child: Slider(
                          activeColor: CLR_DEF_BLUE,
                          thumbColor: selectedType[0] ? CLR_PRESSED : CLR_DEF_BLUE,
                          value: curPeriodSliderValue,
                          max: 17,
                          divisions: 17,
                          label: periods[curPeriodSliderValue.toInt()][2],
                          onChanged: (double value) {
                            if(isStarted){
                              timeStart = DateTime.now();
                            }
                            setState(() {
                              curPeriodSliderValue = value;
                            });
                            savePeriod();
                          },
                        ),
                      ),

                      // * Period label * //
                      Expanded(
                        flex: 1,
                        child:Text(
                          periods[curPeriodSliderValue.toInt()][2],
                          softWrap: false,
                          style: TextStyle(fontSize: 20, fontWeight: FontWeight.bold),
                        ),
                      ),
                    ],
                  ),
                ),
              ),
            ),

            SizedBox(height:paddingMain), // separator

            // * Time section * //
            Expanded(
              flex:4,
              child: Container(
                margin: EdgeInsets.symmetric(horizontal: paddingMain),
                padding: EdgeInsets.all(paddingMain/2),
                decoration: BoxDecoration(
                  color: Color.fromRGBO(80, 80, 80, 1),
                  border: Border.all(color: selectedType[1] ? CLR_PRESSED : Colors.transparent, width: 5),
                  borderRadius: BorderRadius.all(Radius.circular(8)),
                ),
                child: Column(
                  children: [

                    // * Add * //
                    Expanded(
                      flex: 3,
                      child: Row(
                        textDirection: TextDirection.ltr,
                        crossAxisAlignment: CrossAxisAlignment.stretch,
                        mainAxisAlignment: MainAxisAlignment.spaceEvenly,
                        children: <Widget>[

                          // * Add Hours * //
                          ButtonChangeTime(
                            icon: Icons.add_circle_outline_rounded, 
                            funcDown: () {
                              setState(() {buttonPressed = true;});
                              changeTime("hours", "+");
                            },
                            funcUp: () {
                              setState(() {buttonPressed = false;});
                              saveHours();
                            },
                          ),

                          SizedBox(width:paddingMain), //separator
                          
                          // * Add Minutes * //
                          ButtonChangeTime(
                            icon: Icons.add_circle_outline_rounded, 
                            funcDown: () {
                              setState(() {buttonPressed = true;});
                              changeTime("minutes", "+");
                            },
                            funcUp: () {
                              setState(() {buttonPressed = false;});
                              saveMinutes();
                            },
                          ),
                        ],
                      ),
                    ), 

                    SizedBox(height:paddingMain), // separator

                    // * Time labels * //
                    Expanded(
                      flex: 2,
                      child: Container(
                        margin: EdgeInsets.symmetric(horizontal: paddingMain),
                        padding: EdgeInsets.all(paddingMain/2),
                        decoration: BoxDecoration(
                          color: Color.fromRGBO(30, 30, 30, 1),
                          borderRadius: BorderRadius.all(Radius.circular(8)),
                        ),
                        child: Row(
                          textDirection: TextDirection.ltr,
                          crossAxisAlignment: CrossAxisAlignment.stretch,
                          mainAxisAlignment: MainAxisAlignment.spaceEvenly,
                          children: <Widget>[

                            // * Hours label * //
                            Expanded(
                              flex: 2,
                              child: Center(  
                                child: Text(
                                  countHours < 10 ? "0$countHours" : "$countHours",
                                  textAlign: TextAlign.center,
                                  style: TextStyle(fontSize: 70),
                                ), 
                              ),
                            ),

                            // * Time icon * //
                            Expanded(
                              flex: 1,
                                child: Container(
                                  alignment: Alignment.center,
                                  child: Icon(
                                    Icons.watch_outlined,
                                    color: selectedType[1] ? CLR_PRESSED : Colors.white,
                                    size: 60.0,
                                  ),  
                                ),  
                            ),

                            // * Minutes label * //
                            Expanded(
                              flex: 2,
                              child: Center(  
                                child: Text(
                                  countMinutes < 10 ? "0$countMinutes" : "$countMinutes",
                                  textAlign: TextAlign.center,
                                  style: TextStyle(fontSize: 70),
                                ),
                              ),
                            ),
                          ],
                        ),
                      ),
                    ), 

                    SizedBox(height:paddingMain), // separator

                    // * Remove * //
                    Expanded(
                      flex: 3,
                      child: Row(
                        textDirection: TextDirection.ltr,
                        crossAxisAlignment: CrossAxisAlignment.stretch,
                        mainAxisAlignment: MainAxisAlignment.spaceEvenly,
                        children: <Widget>[

                          // * Remove hours * //
                          ButtonChangeTime(
                            icon: Icons.remove_circle_outline_rounded, 
                            funcDown: () {
                              setState(() {buttonPressed = true;});
                              changeTime("hours", "-");
                            },
                            funcUp: () {
                              setState(() {buttonPressed = false;});
                              saveHours();
                            },
                          ),

                          SizedBox(width:paddingMain), // separator

                          // * Remove miutes * //
                          ButtonChangeTime(
                            icon: Icons.remove_circle_outline_rounded, 
                            funcDown: () {
                              setState(() {buttonPressed = true;});
                              changeTime("minutes", "-");
                            },
                            funcUp: () {
                              setState(() {buttonPressed = false;});
                              saveMinutes();
                            },
                          ),
                        ],
                      ),
                    ),
                  ],
                ),
              ),
            ),

            SizedBox(height:paddingMain), // separator

            // * Type & start/pause section * //
            Expanded(
              flex: 2,
              child: 
              Container(
                alignment: Alignment.topLeft,
                padding: EdgeInsets.fromLTRB(paddingMain, 0, paddingMain, paddingMain),
                child: Row(
                  textDirection: TextDirection.ltr,
                  crossAxisAlignment: CrossAxisAlignment.stretch,
                  mainAxisAlignment: MainAxisAlignment.spaceEvenly,
                  children: <Widget>[

                    // * Type * //
                    Expanded(
                      flex: 1,
                      child: Center(  
                        // ignore: avoid_unnecessary_containers
                        child: Container(
                          decoration: BoxDecoration(
                            color: CLR_DEF_BLUE,
                            border: Border.all(color: Color.fromRGBO(50, 50, 50, 0)),
                            borderRadius: BorderRadius.all(Radius.circular(10)),
                          ),
                          child:
                            ToggleButtons(
                              direction: Axis.vertical,
                              onPressed: (int index) {
                                if(isStarted){
                                  return;
                                }
                                setState(() {
                                  // The button that is tapped is set to true, and the others to false.
                                  for (int i = 0; i < selectedType.length; i++) {
                                    selectedType[i] = i == index;
                                  }
                                  saveSelectedType();
                                });
                              },
                              borderColor: Color.fromRGBO(50, 50, 50, 1),
                              borderWidth: 2,
                              borderRadius: const BorderRadius.all(Radius.circular(8)),
                              selectedBorderColor: Color.fromRGBO(50, 50, 50, 1),
                              selectedColor: Colors.white,
                              fillColor: CLR_PRESSED,
                              color: Colors.white,
                              isSelected: selectedType,
                              children: <Widget>[
                                Icon(Icons.timer_outlined),
                                Icon(Icons.watch_outlined),
                              ],
                            ),
                        ), 
                      ),
                    ),

                    SizedBox(width:paddingMain), // separator

                    // * Start/pause * //
                    Expanded(
                      flex: 10,
                      child: Container(
                        padding: EdgeInsets.all(1),
                        decoration: BoxDecoration(
                          color: CLR_DEF_BLUE,
                          borderRadius: BorderRadius.all(Radius.circular(9)),
                        ),
                        child: ElevatedButton.icon(
                          icon: Icon(
                            isStarted ? Icons.pause_circle_outline_rounded : Icons.play_circle_outline_rounded,
                            color: Colors.white,
                            size: 150.0,
                          ),
                          style: ElevatedButton.styleFrom(
                            textStyle: const TextStyle(fontSize: 90),
                            backgroundColor: isStarted ? CLR_PRESSED : CLR_DEF_BLUE,
                            shape: RoundedRectangleBorder(
                              side: BorderSide(color: Color.fromRGBO(50, 50, 50, 1), width: 2),
                              borderRadius: BorderRadius.circular(8), // <-- Radius
                            ),
                            minimumSize: const Size.fromHeight(0)
                          ),
                          onPressed: () async {
                            await startPause();
                          },
                          label: Text(""),
                        ) 
                      ),
                    ),
                  ],
                ),
              ),
            ),
          ],
        ),
      ),
    );
  }
}