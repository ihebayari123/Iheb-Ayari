function validateAddUserForm(event) {
    const name = document.querySelector('input[name="Name"]');
    const surname = document.querySelector('input[name="surname"]');
    const email = document.querySelector('input[name="Email"]');
    const password = document.querySelector('input[name="password"]');

    // Validation du nom
    if (!name.value.trim() || name.value.length < 2) {
        alert("Le nom doit contenir au moins 2 caractères.");
        event.preventDefault();
        return false;
    }

    // Validation du prénom
    if (!surname.value.trim() || surname.value.length < 2) {
        alert("Le prénom doit contenir au moins 2 caractères.");
        event.preventDefault();
        return false;
    }

    // Validation de l'email
    const emailPattern = /^[a-zA-Z0-9._-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,6}$/;
    if (!emailPattern.test(email.value)) {
        alert("Veuillez entrer une adresse email valide.");
        event.preventDefault();
        return false;
    }

    // Validation du mot de passe
    if (password.value.length < 6) {
        alert("Le mot de passe doit contenir au moins 6 caractères.");
        event.preventDefault();
        return false;
    }

    // Si toutes les validations sont passées
    return true;
}

document.addEventListener('DOMContentLoaded', () => {
    // Cibler le formulaire par l'attribut action, si défini, ou un autre moyen
    const addUserForm = document.querySelector('form[action=""]');
    
    if (addUserForm) {
        addUserForm.addEventListener('submit', validateAddUserForm);
    }
});
