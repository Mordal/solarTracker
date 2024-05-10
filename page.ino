
void printContent_Global(WiFiClient client){
  client.print("<html> <head> ");
  client.print(" <style> ");
  client.print("  body {font-family: Arial, Helvetica, sans-serif; margin: 0;} ");
  client.print("  header {background-color: #666; padding: 10px; text-align: center; font-size: 20px; color: white;}") ;
  client.print("  nav {float: left; width: 20%; height: 100%; background: #f1f1f1; padding: 20px;} ");
  client.print("  nav ul {list-style-type: none; padding: 0;} ");
  client.print("  article {float: left; padding: 20px; width: 70%; background-color: #f1f1f1; height: 100%;} ");
  client.print("  section:after {content: \"\"; display: table; clear: both;} ");
  client.print("  footer {background-color: #777; padding: 10px; text-align: center; color: white;} </style> </head> ");
  client.print(" <body> ");
  client.print("   <header> <h2>Solar Tracker</h2> </header> ");
  client.print("   <section> <nav> <ul> ");
  client.print("      <li><a href=\"/\">Home</a></li> ");
  client.print("      <li><a href=\"/Lichtsensors\">Lichtsensors</a></li> ");
  client.print("      <li><a href=\"/Buttons\">Buttons</a></li> </ul> </nav>");
  printContent_Lichtsensors(client);
  printContent_Buttons(client);
  printContent_GlobalEND(client);
  
}
void printContent_GlobalEND(WiFiClient client){
  client.print("</section> <footer> <p>Footer</p> </footer> </body> </html>");
}

void printContent_Lichtsensors(WiFiClient client){
  client.print("<div> <h2>Lichtsensoren</h2> <div>");
  client.print("<p><table><tr>");

  client.print("<th> Links boven: </th>" );
  client.print("<th> Rechts boven: </th>");
  client.print("<th> Links onder: </th>" );
  client.print("<th> Rechts onder: </th>" );
  client.print(" <td>");
  client.print(JSON.stringify(myObject["lichtSensors"]["Links_Boven"])+ "<br>");
  client.print(JSON.stringify(myObject["lichtSensors"]["Rechts_Boven"])+ "<br>");
  client.print(JSON.stringify(myObject["lichtSensors"]["Links_Onder"]) + "<br>");
  client.print(JSON.stringify(myObject["lichtSensors"]["Rechts_Onder"]));
  client.print("</td></tr></table></p> </div> </div>");
  }

void printContent_Movement(WiFiClient client){
  client.print("<div> <h2>Movement</h2> <div>");
  client.print("<p><table><tr><td>");

  client.print("<strong> Links draaien: </strong> <br>" );


  client.print("</td> <td>");
  client.print(JSON.stringify(myObject["TURN_LEFT"]["Moving"])+ "<br>");
}

void printContent_Flags(WiFiClient client){
  client.print("<div> <h2>Flags</h2> <div>");
  client.print("<p><table><tr><td>");

  client.print("<strong> Draaien too long: </strong> <br>" );
  client.print("<strong> Kantelen too long: </strong> <br>");
  client.print("<strong> Safe mode: </strong> <br>" );
  client.print("<strong> Test mode: </strong>" );
  client.print("</td> <td>");
  client.print(JSON.stringify(myObject["Flags"]["Draaien_too_long"])+ "<br>");
  client.print(JSON.stringify(myObject["Flags"]["Kantelen_too_long"])+ "<br>");
  client.print(JSON.stringify(myObject["Flags"]["Safe_mode"]) + "<br>");
  client.print(JSON.stringify(myObject["Flags"]["Test_mode"]));
  client.print("</td></tr></table></p> </div> </div>");
}

void printContent_Buttons(WiFiClient client){
  client.print("<div><strong><a href=\"/JSON\">Get JSON</a> </strong> </div>");
  }
