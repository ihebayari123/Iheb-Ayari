
<?php
// Inclusion des classes nécessaires de PHPMailer
use PHPMailer\PHPMailer\PHPMailer;
use PHPMailer\PHPMailer\Exception;
include '../../Controller/reclamationController.php';
require '../../vendor/autoload.php'; 
session_start();

if (!isset($_SESSION['user_name'])) {
    // Redirect to the signin page if the user is not logged in
    header('Location: signin.php');
    exit();
}
// Vérification si le formulaire a été soumis en méthode POST
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    // Vérification si tous les champs requis sont présents dans le formulaire
    if (isset($_POST['nom'], $_POST['produit'], $_POST['description'], $_POST['date'], $_POST['email'], $_POST['cin'])) {
        // Validation des champs pour s'assurer qu'ils ne sont pas vides
        if (empty($_POST['nom']) || empty($_POST['produit']) || empty($_POST['description']) || empty($_POST['date']) || empty($_POST['email']) || empty($_POST['cin'])) {
            echo "Tous les champs doivent être remplis.";
        } else {
            $nom = $_SESSION['user_name'];
            
            
            // Création d'un objet de réclamation avec les données du formulaire
            $reclamation = new gestion(
                null, // L'ID sera auto-incrémenté dans la base de données
                $nom, 
                $_POST['produit'], 
                $_POST['description'],
                new DateTime($_POST['date']),
                $_POST['email'], 
                $_POST['cin'], 
                
            );

            // Création d'une instance du contrôleur pour ajouter la réclamation dans la base de données
            $reclamationC = new reclamationController();
            $reclamationC->addreclamation($reclamation);

            // Configuration de PHPMailer pour l'envoi de l'email
            $mail = new PHPMailer(true);
            try {
                // Configuration SMTP
                $mail->isSMTP();
                $mail->Host = 'smtp.gmail.com';
                $mail->SMTPAuth = true;
                $mail->Username = 'haningmati3@gmail.com'; // Remplacez par votre email
                $mail->Password = 'wtsr jfqx rfrz qyzq'; // Remplacez par le mot de passe d'application
                $mail->SMTPSecure = PHPMailer::ENCRYPTION_STARTTLS;
                $mail->Port = 587;

                // Paramètres de l'email
                $mail->setFrom('AgriAura@gmail.com', 'AgriAura'); // Adresse de l'expéditeur
                $mail->addAddress('haningmati3@gmail.com', 'Administrateur'); // Adresse du destinataire
                $mail->addReplyTo($_POST['email'], $_POST['nom']); // Répondre à l'email de l'utilisateur

                // Contenu de l'email en HTML
                $mail->isHTML(true);
                $mail->Subject = 'Nouvelle réclamation soumise';
                $mail->Body = "
                    <p>Une nouvelle réclamation a été soumise :</p>
                    <ul>
                        <li><b>Nom :</b> {$_POST['nom']}</li>
                        <li><b>Produit :</b> {$_POST['produit']}</li>
                        <li><b>Description :</b> {$_POST['description']}</li>
                        <li><b>Date :</b> {$_POST['date']}</li>
                        <li><b>Email :</b> {$_POST['email']}</li>
                        <li><b>CIN :</b> {$_POST['cin']}</li>
                    </ul>
                ";

                // Envoi de l'email
                $mail->send();

                // Confirmation de l'ajout de la réclamation et de l'envoi de l'email
                echo "Réclamation ajoutée et email envoyé à l'administrateur.";

                // Redirection vers la page de confirmation
                header('Location: confirmation.php');
                exit; // Assurez-vous que le script s'arrête ici pour éviter l'exécution de lignes supplémentaires

            } catch (Exception $e) {
                // Gestion des erreurs lors de l'envoi de l'email
                echo "Erreur lors de l'envoi de l'email : {$mail->ErrorInfo}";
            }
        }
    } else {
        // Si le formulaire n'est pas complet
        echo "Tous les champs doivent être remplis.";
    }
}
?>





<!DOCTYPE HTML>

<html>
	<head>
		<title>Réclamations - AgriAura</title>
		
		<meta charset="utf-8" />
		<meta name="viewport" content="width=device-width, initial-scale=1, user-scalable=no" />
		<link rel="stylesheet" href="assets/css/main.css" />
		<noscript><link rel="stylesheet" href="assets/css/noscript.css" /></noscript>
       
	</head>
	<body class="is-preload">

		<!-- Wrapper -->
			<div id="wrapper">

				<!-- Header -->
					<header id="header">
						<div class="logo">
							<span class="icon fa-gem"></span>
						</div>
                        
						<nav>
							<ul>
                                <li><a href="index.php">Home</a></li>
								<li><a href="#contact">RECLAMATION</a></li>
								
								<!--<li><a href="#elements">Elements</a></li>-->
							</ul>
						</nav>
                      
					</header>
                    
				<!-- Main -->
					<div id="main">
                    
						
						<!-- Contact -->
							<article id="contact">
                        					<!-- Contact Section -->
   <section id="contact" class="contact section">
    <h2>FORMULAIRE DE RECLAMATION</h2>
   <h4>Votre plateforme de gestion agricole durable et intelligente</h4>
      <div class="container">
        <div class="col-lg-8 mx-auto">
          <form action="" method="post" id="reclamationForm">
            <div class="row">
              <div class="col-md-6 form-group">
              <div class="form-group mt-3">
    <label for="nom" style="font-weight: bold;">Nom</label>
    <input type="text" class="form-control" name="nom" id="nom" 
           style="width: 100%; padding: 10px; font-size: 16px; border: 1px solid #ccc;" 
           value="<?php echo htmlspecialchars($_SESSION['user_name']); ?>" readonly>
</div>

              <div class="col-md-6 form-group">
                <label for="produit" style="font-weight: bold;">Produit</label>
                <input type="text" name="produit" class="form-control" id="produit" placeholder="Entrez votre produit"  style="width: 100%; padding: 10px; font-size: 16px; border: 1px solid #ccc;">
                
                <div id="produitError" class="error-message text-danger"></div>
              </div>
            </div>

            <div class="form-group mt-3">
              <label for="description" style="font-weight: bold;">Description</label>
              <textarea class="form-control" name="description" id="description" placeholder="Décrivez votre réclamation"  style="width: 100%; padding: 10px; font-size: 16px; border: 1px solid #ccc;"></textarea>
              <div id="descriptionError" class="error-message text-danger"></div>
            </div>

            <div class="form-group mt-3">
  <label for="date" style="font-weight: bold;">Date</label>
  <input type="date" class="form-control" name="date" id="date" 
         style="width: 100%; padding: 10px; font-size: 16px; border: 1px solid #ccc; color: black; background-color: white;"  style="width: 100%; padding: 10px; font-size: 16px; border: 1px solid #ccc;">
</div>

            <!-- Nouvelle entrée pour l'email -->
            <div class="form-group mt-3">
              <label for="email" style="font-weight: bold;">Email</label>
              <input type="email" class="form-control" name="email" id="email" placeholder="Entrez votre email"   style="width: 100%; padding: 10px; font-size: 16px; border: 1px solid #ccc;">
              <div id="emailError" class="error-message text-danger"></div>
            </div>

            <!-- Nouvelle entrée pour le CIN -->
            <div class="form-group mt-3">
              <label for="cin" style="font-weight: bold;">CIN</label>
              <input type="text" class="form-control" name="cin" id="cin" placeholder="Entrez votre CIN"  style="width: 100%; padding: 10px; font-size: 16px; border: 1px solid #ccc;">
              <div id="cinError" class="error-message text-danger"></div>
            </div>
            <div class="col-md-12">
                              <br>    
            <div class="text-center mt-4">
    <button type="submit" class="btn btn-primary hvr-hover">Ajouter une réclamation</button>
</div>


            
          </form>
        </div>
      </div>
      
    </section>
								
    </div>
				<!-- Footer -->
				<footer id="footer">
					<p class="copyright">&copy; by NextGen Creators .</p>
				</footer>

			</div>

		<!-- BG -->
			<div id="bg"></div>

		<!-- Scripts -->
			<script src="assets/js/jquery.min.js"></script>
			<script src="assets/js/browser.min.js"></script>
			<script src="assets/js/breakpoints.min.js"></script>
			<script src="assets/js/util.js"></script>
			<script src="assets/js/main.js"></script>
			
            <script>
    document.addEventListener('DOMContentLoaded', () => {
        const form = document.getElementById('reclamationForm');

        form.addEventListener('submit', function(event) {
            event.preventDefault(); // Empêche la soumission immédiate

            let valid = true;
            
            // Récupérer les valeurs des champs
            const nom = document.getElementById('nom').value.trim();
            const produit = document.getElementById('produit').value.trim();
            const description = document.getElementById('description').value.trim();
            const date = document.getElementById('date').value.trim();
            const email = document.getElementById('email').value.trim();
            const cin = document.getElementById('cin').value.trim();

            // Réinitialiser les messages précédents
            resetMessages();

            // Validation des champs
            if (nom.length < 3) {
                showError('nom', 'Le nom est obligatoire avec une longueur minimale de 3 caractères.');
                valid = false;
            } 

            if (produit.length < 3) {
                showError('produit', 'Le produit doit avoir une longueur minimale de 3 caractères.');
                valid = false;
            }

            const descriptionLength = description.length;
            if (descriptionLength < 5 || descriptionLength > 255) {
                showError('description', 'La description doit comporter entre 5 et 255 caractères.');
                valid = false;
            }

            if (!isValidDate(date)) {
                showError('date', 'Veuillez entrer une date valide.');
                valid = false;
            }

            const emailRegex = /^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$/;
            if (!emailRegex.test(email)) {
                showError('email', 'Veuillez entrer un email valide.');
                valid = false;
            }

            // Contrôle de saisie pour le CIN
            if (cin.length !== 8 || isNaN(cin)) {
                showError('cin', 'Le CIN doit être composé de 8 chiffres.');
                valid = false;
            }

            // Si tout est valide, afficher l'alerte et soumettre
            if (valid) {
                alert('Votre réclamation a été envoyée avec succès.');
                form.submit();
            }
        });

        function resetMessages() {
            document.querySelectorAll('.error-message').forEach(msg => msg.innerText = '');
        }

        function showError(field, message) {
            const errorField = document.getElementById(`${field}Error`);
            errorField.innerText = message;
        }

        function isValidDate(date) {
            return !isNaN(Date.parse(date));
        }
    });
  </script>
</body>

</html>

	
