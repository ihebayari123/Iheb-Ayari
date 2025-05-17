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
<?php
include '../../Controller/reclamationController.php';

if (isset($_GET['id'])) {
    $id = $_GET['id'];
    $gestionreclamationC = new reclamationController();
    
    // Récupérer les informations actuelles de la résolution
    $resolution = $gestionreclamationC->getResolutionById($id);
}

if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    // Récupérer les données soumises
    $statut = $_POST['statut'];
    $assigned_to = $_POST['assigned_to'];
    $description = $_POST['description'];
    $method = $_POST['method'];
    $follow_up_date = $_POST['follow_up_date'];
    $resolution_date = $_POST['resolution_date'];
    
    // Mettre à jour la résolution
    $gestionreclamationC->updateResolution($id, $statut, $assigned_to, $description, $method, $follow_up_date, $resolution_date);
    
    // Rediriger après la mise à jour
    header('Location: reclamationlist.php');
}
?>

<form method="POST" onsubmit="return validateForm();">
    <label for="statut">Statut</label>
    <select id="statut" name="statut">
        <option value="Résolu" <?php echo $resolution['statut'] === 'Résolu' ? 'selected' : ''; ?>>Résolu</option>
        <option value="Non Résolu" <?php echo $resolution['statut'] === 'Non Résolu' ? 'selected' : ''; ?>>Non Résolu</option>
        <option value="En Cours" <?php echo $resolution['statut'] === 'En Cours' ? 'selected' : ''; ?>>En Cours</option>
    </select>

    <label for="assigned_to">Assigned To</label>
    <input type="text" id="assigned_to" name="assigned_to" value="<?php echo $resolution['assigned_to']; ?>">

    <label for="description">Description</label>
    <textarea id="description" name="description"><?php echo $resolution['resolution_description']; ?></textarea>

    <label for="method">Méthode</label>
    <input type="text" id="method" name="method" value="<?php echo $resolution['resolution_method']; ?>">

    <label for="follow_up_date">Date Suivi</label>
    <input type="date" id="follow_up_date" name="follow_up_date" value="<?php echo $resolution['follow_up_date']; ?>">

    
    <button type="submit">Mettre à jour</button>
</form>

<script>
function validateForm() {
    // Récupérer les valeurs des champs
    const statut = document.getElementById('statut').value;
    const assignedTo = document.getElementById('assigned_to').value.trim();
    const description = document.getElementById('description').value.trim();
    const method = document.getElementById('method').value.trim();
    const followUpDate = document.getElementById('follow_up_date').value;
    

    const today = new Date().toISOString().split('T')[0]; // Date actuelle au format AAAA-MM-JJ

    // Vérifier le champ "Statut"
    if (!['Résolu', 'Non Résolu', 'En Cours'].includes(statut)) {
        alert('Le champ "Statut" doit être "Résolu", "Non Résolu" ou "En Cours".');
        return false;
    }

    // Vérifier "Assigned To" (minimum 2 lettres)
    if (assignedTo.length < 2) {
        alert('Le champ "Assigned To" doit contenir au moins 2 lettres.');
        return false;
    }

    // Vérifier "Description" (minimum 4 lettres)
    if (description.length < 2) {
        alert('Le champ "Description" doit contenir au moins 2 lettres.');
        return false;
    }

    // Vérifier "Méthode" (minimum 4 lettres)
    if (method.length < 2) {
        alert('Le champ "Méthode" doit contenir au moins 2 lettres.');
        return false;
    }

    // Vérifier "Date de Suivi" (ne doit pas être dans le passé)
    if (followUpDate < today || followUpDate> today ) {
        alert('La "Date de Suivi" ne peut pas être dans le passé OU dans le futur.');
        return false;
    }

    

    return true; // Le formulaire est valide
}
</script>
