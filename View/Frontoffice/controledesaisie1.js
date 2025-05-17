document.addEventListener("DOMContentLoaded", function () {
    // Gestion du formulaire de connexion
    const signinForm = document.querySelector("form"); // Récupère le formulaire de connexion

    signinForm.addEventListener("submit", function (event) {
        // Récupération des champs
        const email = document.getElementById("email").value.trim();
        const password = document.getElementById("password").value.trim();

        // Regex pour validation email
        const emailRegex = /^[^\s@]+@[^\s@]+\.[^\s@]+$/;

        // Initialisation d'une variable pour suivre les erreurs
        let isValid = true;
        let errorMessage = "";

        // Validation du champ email
        if (email === "" || !emailRegex.test(email)) {
            isValid = false;
            errorMessage += "Veuillez entrer une adresse email valide.\n";
        }

        // Validation du champ mot de passe
        if (password === "" || password.length < 6) {
            isValid = false;
            errorMessage += "Le mot de passe doit comporter au moins 6 caractères.\n";
        }

        // Affichage des erreurs et annulation de l'envoi
        if (!isValid) {
            alert(errorMessage);
            event.preventDefault(); // Empêche l'envoi du formulaire
        }
    });

    // Gestion du formulaire d'inscription (ajout de la validation si besoin)
    const form = document.querySelector("form"); // Formulaire d'inscription pour réutiliser la logique

    form.addEventListener("submit", function (event) {
        // Récupération des champs
        const name = document.getElementById("Name").value.trim();
        const surname = document.getElementById("surname").value.trim();
        const email = document.getElementById("Email").value.trim();
        const password = document.getElementById("password").value.trim();

        // Regex pour validation email
        const emailRegex = /^[^\s@]+@[^\s@]+\.[^\s@]+$/;

        // Initialisation d'une variable pour suivre les erreurs
        let isValid = true;
        let errorMessage = "";

        // Validation des champs
        if (name === "") {
            isValid = false;
            errorMessage += "Le nom est requis.\n";
        }

        if (surname === "") {
            isValid = false;
            errorMessage += "Le prénom est requis.\n";
        }

        if (email === "" || !emailRegex.test(email)) {
            isValid = false;
            errorMessage += "Veuillez entrer une adresse email valide.\n";
        }

        if (password === "" || password.length < 6) {
            isValid = false;
            errorMessage += "Le mot de passe doit comporter au moins 6 caractères.\n";
        }

        // Affichage des erreurs et annulation de l'envoi
        if (!isValid) {
            alert(errorMessage);
            event.preventDefault(); // Empêche l'envoi du formulaire
        }
    });
});
