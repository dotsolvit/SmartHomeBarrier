#include "sh_sendhtml.hpp"

//Функція для генерації HTML-сторінки (Function for generating an HTML page)
String SendHTML(String mess) {
  String html = R"(
    <!DOCTYPE html>
    <html lang="en">
    <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>Smart Home Barrier</title>

        <style>
          body {
            text-align: center;
          }  
          button {
            width: 70px;
            margin: 10px;
            cursor: pointer; 
            box-shadow: 0 4px 6px rgba(0, 0, 0, 0.2); 
          }
          p {
            font-size: 120%;
            margin: 0 auto;
            width: 300px;
            border: 1px solid;
            padding: 10px;
            border-radius: 8px;
            box-shadow: 0 4px 6px rgba(0, 0, 0, 0.2);
          }
        </style>
        
    </head>
    <body>
        <h2>Smart Home Barrier</h2>
        <br>
        <div>
          <button onclick="location.href='/open'">OPEN</button>
          <button onclick="location.href='/close'">CLOSE</button>
          <button onclick="location.href='/block'">BLOCK</button>
        </div>
        <br>
        <p>Stage: )" + mess + R"(</p>
        <br>
        <hr width="320px">

        <!-- Автоматичне оновлення сторінки кожні 2000мс Automatic page refresh every 2000ms -->
        <script> setTimeout(function(){location.reload(); }, 2000); </script>

    </body>
    </html>
  )";
  return html; //Повернення готової HTML-сторінки (Returning the finished HTML page)
}