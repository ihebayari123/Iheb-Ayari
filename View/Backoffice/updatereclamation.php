<!DOCTYPE html>
<html lang="fr">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Mise à jour Résolution</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            margin: 0;
            padding: 0;
            background-color: #f9f9f9;
            color: #333;
        }

        form {
            max-width: 600px;
            margin: 50px auto;
            padding: 20px;
            background: #ffffff;
            border-radius: 8px;
            box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1);
        }

        label {
            display: block;
            margin-bottom: 8px;
            font-weight: bold;
            color: #555;
        }

        input, select, textarea, button {
            width: 100%;
            padding: 10px;
            margin-bottom: 15px;
            border: 1px solid #ddd;
            border-radius: 5px;
            font-size: 16px;
        }

        textarea {
            resize: none;
            height: 100px;
        }

        button {
            background-color: #28a745;
            color: white;
            border: none;
            cursor: pointer;
            font-size: 18px;
        }

        button:hover {
            background-color: #218838;
        }

        .form-title {
            text-align: center;
            font-size: 24px;
            margin-bottom: 20px;
            color: #333;
        }

        .error {
            color: red;
            font-size: 14px;
            margin-top: -10px;
            margin-bottom: 10px;
        }
    </style>
</head>
<body>
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Document</title>
</head>
<body>
<a href="resolvereclamation.php?id=93">modifier la réclamation</a>

</body>
</html>
<?php
include('../../Controller/reclamationController.php');

if (isset($_GET['id'])) {
    $id = $_GET['id'];

    // Créer une instance du contrôleur
    $reclamationController = new reclamationController();

    // Récupérer la réclamation à partir de la base de données
    $reclamation = $reclamationController->getreclamation($id);
} else {
    echo "ID non spécifié!";
    exit();
}
?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Modifier Réclamation</title>
</head>
<body>
    <h1>Modifier Réclamation</h1>
    <form id="reclamationForm" action="updatereclamation_process.php" method="POST">
        <input type="hidden" name="id" value="<?= $reclamation['id'] ?>">

        <label for="nom">Nom :</label>
        <input type="text" id="nom" name="nom" value="<?= $reclamation['nom'] ?>"><br><br>
        <div id="nomMessage"></div>

        <label for="produit">Produit :</label>
        <input type="text" id="produit" name="produit" value="<?= $reclamation['produit'] ?>"><br><br>
        <div id="produitMessage"></div>

        <label for="description">Description :</label>
        <textarea id="description" name="description"><?= $reclamation['description'] ?></textarea><br><br>
        <div id="descriptionMessage"></div>

        <label for="date">Date :</label>
        <input type="date" id="date" name="date" value="<?= $reclamation['date'] ?>"><br><br>
        <div id="dateMessage"></div>

        <label for="statut">Statut :</label>
        <select id="statut" name="statut">
            <option value="En cours" <?= $reclamation['statut'] == 'En cours' ? 'selected' : '' ?>>En cours</option>
            <option value="Résolu" <?= $reclamation['statut'] == 'Résolu' ? 'selected' : '' ?>>Résolu</option>
            <option value="Non résolu" <?= $reclamation['statut'] == 'Non résolu' ? 'selected' : '' ?>>Non résolu</option>
        </select><br><br>
        <div id="statutMessage"></div>

        <button type="submit">Mettre à jour</button>
    </form>

    <script>
        document.addEventListener('DOMContentLoaded', () => {
            const form = document.getElementById('reclamationForm');

            form.addEventListener('submit', function(event) {
                event.preventDefault(); 

                let valid = true;
                let alertMessage = ""; // Contiendra les messages d'erreur à afficher dans une alerte

                // Récupérer les valeurs des champs
                const nom = document.getElementById('nom').value.trim();
                const produit = document.getElementById('produit').value.trim();
                const description = document.getElementById('description').value.trim();
                const date = document.getElementById('date').value.trim();
                const statut = document.getElementById('statut').value.trim();

                // Réinitialiser les messages précédents
                resetMessages();

                // Validation du champ 'nom' (obligatoire, longueur min 3)
                if (nom.length < 3) {
                    showError('nom', 'Le nom est obligatoire avec une longueur minimale de 3 caractères.');
                    valid = false;
                } else {
                    showSuccess('nom', 'Nom valide.');
                }

                // Validation du champ 'produit' (obligatoire, longueur min 3)
                if (produit.length < 3) {
                    showError('produit', 'Le produit doit avoir une longueur minimale de 3 caractères.');
                    valid = false;
                } else {
                    showSuccess('produit', 'Produit valide.');
                }

                // Validation du champ 'description' (obligatoire, longueur min 3, uniquement lettres et espaces)
                const descriptionRegex = /^[A-Za-z\s]+$/;
                if (description.length < 3 || !descriptionRegex.test(description)) {
                    showError('description', 'La description doit être d\'au moins 3 caractères et ne doit contenir que des lettres et des espaces.');
                    valid = false;
                } else {
                    showSuccess('description', 'Description valide.');
                }

                // Validation du champ 'date' (obligatoire et pas dans le futur)
                if (!date) {
                    showError('date', 'La date est obligatoire.');
                    valid = false;
                } else {
                    const currentDate = new Date();
                    const inputDate = new Date(date);
                    if (inputDate > currentDate) {
                        showError('date', 'La date ne peut pas être dans le futur.');
                        valid = false;
                    } else {
                        showSuccess('date', 'Date valide.');
                    }
                }

                // Validation du champ 'statut' (doit être parmi une liste de valeurs)
                const validStatuses = ['résolu', 'en attente', 'en cours', 'non résolu'];
                if (!validStatuses.includes(statut.toLowerCase())) {
                    showError('statut', 'Le statut doit être "résolu", "en attente", "en cours", ou "non résolu".');
                    valid = false;
                } else {
                    showSuccess('statut', 'Statut valide.');
                }

                // Si le formulaire est valide, le soumettre
                if (valid) {
                    form.submit();
                } else {
                    alert("Veuillez corriger les erreurs avant de soumettre le formulaire.");
                }
            });

            // Fonction pour afficher un message d'erreur
            function showError(fieldId, message) {
                const messageElement = document.getElementById(fieldId + "Message");
                messageElement.innerHTML = `<span style="color:red">${message}</span>`;
            }

            // Fonction pour afficher un message de succès
            function showSuccess(fieldId, message) {
                const messageElement = document.getElementById(fieldId + "Message");
                messageElement.innerHTML = `<span style="color:green">${message}</span>`;
            }

            // Fonction pour réinitialiser les messages d'erreur et de succès
            function resetMessages() {
                const messages = document.querySelectorAll('[id$="Message"]');
                messages.forEach(message => message.innerHTML = ''); // Supprimer tous les messages existants
            }
        });
    </script>

</body>
</html>
