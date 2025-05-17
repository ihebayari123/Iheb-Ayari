<?php
require_once "../controller/reclamationController.php";

$reclamationController = new reclamationController();
$results = []; // Stocke les résultats de la recherche

// Vérifier si le formulaire a été soumis
if ($_SERVER["REQUEST_METHOD"] === "POST" && isset($_POST['search'])) {
    $field = $_POST['searchField']; // Champ à rechercher (ex. : "produit")
    $value = $_POST['searchValue']; // Valeur à rechercher (nom du produit)

    // Vérification du champ pour éviter les injections SQL
    $allowedFields = ['produit'];
    if (in_array($field, $allowedFields)) {
        $results = $reclamationController->searchReclamations($field, $value);
    } else {
        echo "<p style='color: red;'>Critère de recherche invalide.</p>";
    }
}

// Afficher les résultats
?>
<!DOCTYPE html>
<html lang="fr">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Résultats de la recherche</title>
</head>
<body>
    <h1>Résultats de la recherche</h1>
    <?php if (!empty($results)): ?>
        <table border="1">
            <thead>
                <tr>
                    <th>ID</th>
                    <th>Produit</th>
                    <th>Description</th>
                    <th>Statut</th>
                </tr>
            </thead>
            <tbody>
                <?php foreach ($results as $reclamation): ?>
                    <tr>
                        <td><?php echo htmlspecialchars($reclamation['id']); ?></td>
                        <td><?php echo htmlspecialchars($reclamation['produit']); ?></td>
                        <td><?php echo htmlspecialchars($reclamation['description']); ?></td>
                        <td><?php echo htmlspecialchars($reclamation['statut']); ?></td>
                    </tr>
                <?php endforeach; ?>
            </tbody>
        </table>
    <?php else: ?>
        <p>Aucun résultat trouvé pour ce produit.</p>
    <?php endif; ?>
</body>
</html>
