#ifndef _HTML_FILES_H_
#define _HTML_FILES_H_

#include <StreamString.h>

// const String response = "{ \"motor1\":{MOTOR1} ,\"motor2\":{MOTOR2},\"stepCount\":{COUNT}, \"status\":\"success\"}";
const String response = "<!DOCTYPE html>\n \
<html lang=\"en\">\n \
<head>\n \
    <meta charset=\"UTF-8\">\n \
    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n \
    <title>Update Success</title>\n \
    <script>\n \
	    let coundown = 5; \n \
		let messageDiv = null;\n \
		const msg = \"Taking you back to the config page in \"; \n \
        document.addEventListener(\"DOMContentLoaded\", function() {\n \
			messageDiv = document.getElementById(\"message\");\n \
            messageDiv.textContent = `${msg}${coundown}`;\n \
            setInterval(function() {\n \
				coundown--; \n \
                if (coundown < 0) \n \
				    window.history.back();\n \
                else \n \
                    messageDiv.textContent = `${msg}${coundown}`;\n \
				}, 1000);\n \
        });\n \
    </script>\n \
</head>\n \
<body>\n \
    <h1>Success!!!</h1>\n \
    <div id=\"message\"></div>\n \
</body>\n \
</html>";

const String configPage = "<!DOCTYPE html> \n  \
<html> \n  \
<head> \n  \
  <title>Motor Control Form</title> \n  \
  <script> \n  \
    function setHiddenValues() { \n  \
      document.getElementById(\"hiddenMotor1\").value = document.getElementById(\"motor1\").checked ? 1 : 0; \n  \
      document.getElementById(\"hiddenMotor2\").value = document.getElementById(\"motor2\").checked ? 1 : 0; \n  \
    } \n  \
  </script> \n  \
      <style> \n  \
           body {  \
                font-family: Arial, sans-serif;  \
        }  \
        /* Header styling */ \n  \
        .header { \n  \
            position: fixed; \n  \
            top: 0; \n  \
            left: 0; \n  \
            width: 100%; \n  \
            padding: 10px; \n  \
            background-color: #f1f1f1; \n  \
            text-align: left; \n  \
            box-shadow: 0 1px 5px rgba(0, 0, 0, 0.1); \n  \
        } \n  \
        /* Flexbox container for the two columns */ \n  \
        .container { \n  \
            display: flex; \n  \
        	margin-top:50px; \n  \
        } \n  \
        /* First column for local elements */ \n  \
        .local-elements { \n  \
            flex: 1; \n  \
            padding: 10px; \n  \
        	margin-left:200px; \n  \
        	margin-top:50px; \n  \
        } \n  \
        /* Second column for the iframe */ \n  \
        .iframe-container { \n  \
            flex: 1; \n  \
            padding: 10px; \n  \
        	margin-right:200px; \n  \
        } \n  \
        .center-title { \n  \
            text-align: center; \n  \
        	font-size: 60px; \n  \
        	font-weight: bold; \n  \
        } \n  \
        /* Footer styling */ \n  \
        .footer { \n  \
            position: fixed; \n  \
            bottom: 0; \n  \
            left: 0; \n  \
            width: 100%; /* Optional: Adjust width if needed */ \n  \
            padding: 10px; \n  \
            background-color: #f1f1f1; \n  \
            text-align: left; \n  \
            box-shadow: 0 -1px 5px rgba(0, 0, 0, 0.1); \n  \
        } \n  \
        </style> \n  \
</head> \n  \
<body> \n  \
  <div class=\"header center-title\">ESP Curtain Control</div> \n  \
  <div class=\"container\"> \n  \
    <!-- First column for local elements --> \n  \
    <div class=\"local-elements\"> \n  \
      <h1>Motor Control</h1> \n  \
      <form action=\"/configUpdate\" method=\"POST\" onsubmit=\"setHiddenValues()\"> \n  \
        <label for=\"motor1\">Motor1:</label> \n  \
        <input type=\"checkbox\" id=\"motor1\" name=\"motor1\" {MOTOR1_BOOL} ><br><br> \n  \
         \n  \
        <label for=\"motor2\">Motor2:</label> \n  \
        <input type=\"checkbox\" id=\"motor2\" name=\"motor2\" {MOTOR2_BOOL} ><br><br> \n  \
         \n  \
        <input type=\"hidden\" id=\"hiddenMotor1\" name=\"motor1\" value=\"{MOTOR1}\"> \n  \
        <input type=\"hidden\" id=\"hiddenMotor2\" name=\"motor2\" value=\"{MOTOR2}\"> \n  \
         \n  \
        <label for=\"stepCount\">Step Count (max 100000):</label> \n  \
        <input type=\"number\" id=\"stepCount\" name=\"stepCount\" max=\"100000\" min=\"1000\" value=\"{COUNT}\"><br><br> \n  \
         \n  \
        <input type=\"submit\" value=\"Submit\"> \n  \
      </form> \n  \
       \n  \
    </div> \n  \
 \n  \
    <!-- Second column for iframe --> \n  \
    <div class=\"iframe-container\"> \n  \
      <iframe src=\"/update\" width=\"100%\" height=\"700px\" frameborder=\"0\"></iframe> \n  \
    </div> \n  \
  </div> \n  \
   \n  \
  <footer class=\"footer\"> \n  \
    <h4>{VERSION}</h4> \n  \
  </footer> \n  \
</body> \n  \
</html>";

#endif //!_HTML_FILES_H_