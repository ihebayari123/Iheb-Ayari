<?php
include '../../Controller/reclamationController.php';

if (isset($_GET['id'])) {
    $id = $_GET['id'];
    $gestionreclamationC = new reclamationController();

    // Récupérer la réclamation en cours pour pré-remplir le formulaire
    $reclamation = $gestionreclamationC->getreclamation($id);
}

// Vérifier si le formulaire a été soumis
if ($_SERVER['REQUEST_METHOD'] == 'POST') {
    $statut = $_POST['statut'];
    $assigned_to = $_POST['assigned_to'];
    $resolution_description = $_POST['resolution_description'];
    $resolution_method = $_POST['resolution_method'];
    $follow_up_date = $_POST['follow_up_date'];

    // Appeler la méthode pour enregistrer la réclamation résolue
    $isResolved = $gestionreclamationC->resolverReclamationAndSave(
        $id,
        $statut,
        $assigned_to,
        $resolution_description,
        $resolution_method,
        $follow_up_date
    );

    if ($isResolved) {
        $message = "La réclamation a été résolue et enregistrée avec succès.";
    } else {
        $message = "Une erreur est survenue, la réclamation n'a pas pu être résolue.";
    }
}
?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Résoudre une Réclamation</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            background-color: #f8f9fa;
            display: flex;
            justify-content: center;
            align-items: center;
            height: 100vh;
            margin: 0;
        }
        .container {
            background-color: #fff;
            padding: 20px 30px;
            border-radius: 8px;
            box-shadow: 0 4px 10px rgba(0, 0, 0, 0.1);
            text-align: center;
            max-width: 500px;
            width: 100%;
        }
        h1 {
            font-size: 22px;
            color: #333;
            margin-bottom: 10px;
        }
        p {
            font-size: 16px;
            color: #555;
            margin-bottom: 20px;
        }
        .success {
            color: #28a745;
        }
        .error {
            color: #dc3545;
        }
        form {
            text-align: left;
        }
        label {
            display: block;
            margin: 8px 0 5px;
        }
        input, select, textarea {
            width: 100%;
            padding: 8px;
            margin-bottom: 15px;
            border: 1px solid #ccc;
            border-radius: 4px;
        }
        .btn-submit {
            background-color: #007bff;
            color: white;
            padding: 10px 20px;
            border-radius: 5px;
            font-size: 16px;
            cursor: pointer;
        }
        .btn-submit:hover {
            background-color: #0056b3;
        }
    </style>
<script>
        function validateForm() {
    // Récupérer les valeurs des champs
    var statut = document.getElementById("statut").value;
    var assigned_to = document.getElementById("assigned_to").value.trim();
    var resolution_description = document.getElementById("resolution_description").value.trim();
    var resolution_method = document.getElementById("resolution_method").value.trim();
    var follow_up_date = document.getElementById("follow_up_date").value;

    var today = new Date().toISOString().split('T')[0]; // Date actuelle au format yyyy-mm-dd

    // Vérification du champ "Statut"
    if (!['Résolu', 'Non Résolu', 'En Cours'].includes(statut)) {
        alert('Le champ "Statut" doit être "Résolu", "Non Résolu" ou "En Cours".');
        return false;
    }

    // Vérifier le champ "Assigned To" (minimum 2 caractères)
    if (assigned_to.length < 2) {
        alert('Le champ "Assigné à" doit contenir au moins 2 lettres.');
        return false;
    }

    // Vérifier la "Description de la résolution" (minimum 4 caractères)
    if (resolution_description.length < 2) {
        alert('La "Description de la résolution" doit contenir au moins 2 lettres.');
        return false;
    }

    // Vérifier la "Méthode de résolution" (minimum 4 caractères)
    if (resolution_method.length < 2) {
        alert('La "Méthode de résolution" doit contenir au moins 2 lettres.');
        return false;
    }

    // Vérifier la "Date de suivi" (ne doit pas être dans le passé)
    if (follow_up_date < today || follow_up_date > today ) {
        alert('La "Date de suivi" ne peut pas être dans le passé ou dans le futur.');
        return false;
    }

    return true; // Permet l'envoi du formulaire
}
</script>
</head>
<body>
    <div class="container">
        <h1>Résoudre la réclamation</h1>
        
        <?php if (isset($message)): ?>
            <p class="<?php echo ($isResolved) ? 'success' : 'error'; ?>"><?php echo $message; ?></p>
        <?php endif; ?>
        
        <form method="POST" action="resolvereclamation.php?id=<?php echo $id; ?>" onsubmit="return validateForm()">
            <label for="statut">Statut de la réclamation</label>
            <select name="statut" id="statut">
                <option value="Résolu" selected>Résolu</option>
            </select>
            
            <label for="assigned_to">Assigné à</label>
            <input type="text" name="assigned_to" id="assigned_to" value="Admin" >

            <label for="resolution_description">Description de la résolution</label>
            <textarea name="resolution_description" id="resolution_description"></textarea>

            <label for="resolution_method">Méthode de résolution</label>
            <textarea name="resolution_method" id="resolution_method"></textarea>

            <label for="follow_up_date">Date de suivi</label>
            <input type="text" name="follow_up_date" id="follow_up_date" placeholder="YYYY-MM-DD">

            <button type="submit" class="btn-submit">Soumettre la résolution</button>
        </form>
        
        <a href="reclamationlist.php">Retour à la liste des réclamations</a>
    </div>
</body>
</html>
