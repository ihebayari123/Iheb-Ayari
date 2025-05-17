<!-- Formulaire pour rechercher une réclamation -->
<form action="searchResults.php" method="POST">
    <label for="searchField">Rechercher par :</label>
    <select name="searchField" id="searchField">
        <option value="produit">Produit</option>
    </select>

    <input type="text" name="searchValue" placeholder="Nom du produit" required>

    <button type="submit" name="search">Rechercher</button>
</form>
