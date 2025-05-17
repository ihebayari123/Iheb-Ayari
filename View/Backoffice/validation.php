<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Résolution</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            text-align: center;
            margin-top: 50px;
        }
        button {
            padding: 10px 20px;
            font-size: 16px;
            background-color: #007bff;
            color: white;
            border: none;
            border-radius: 5px;
            cursor: pointer;
        }
        button:hover {
            background-color: #0056b3;
        }
    </style>
</head>
<body>
<h3>"Votre resolution a été envoyée avec succès.
               email envoyé au client. 
        
        <br>~_~</h3>
        
    <div class="text-center mt-4">
        <button id="closeButton" class="btn btn-primary">OK</button>
    </div>

    <script>
        document.getElementById('closeButton').addEventListener('click', function() {
            // Si cette page est ouverte dans un onglet/fenêtre, elle fermera.
            // Sinon, on redirige vers une autre page par défaut.
            if (window.history.length > 1) {
                window.history.back(); // Revenir à la page précédente.
            } else {
                window.close(); 
            }
        });
    </script>
</body>
</html>
