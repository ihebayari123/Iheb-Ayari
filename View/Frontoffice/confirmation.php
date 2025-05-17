<!DOCTYPE HTML>

<html>
	<head>
		<title>Réclamations - AgriAura</title>
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
						
						<h4>CONTACT US<H4>
					</header>

				<!-- Main -->
					<div id="main">

						
						<!-- Contact -->
							<article id="contact">
								
								<h2 class="major">welcome</h2>
								<form method="post" action="#">
									<body>
										<h3>"Votre réclamation a été envoyée avec succès.
												   email envoyé à l'administrateur. 
											<br>Nous vous répondrons bientôt."</h3>
											
										
										<a href="addgestionreclamation.php" class="btn btn-secondary custom-btn">
  exit
</a>

<style>
  .custom-btn {
    background-color: #f0f0f0; /* Couleur d'arrière-plan personnalisée */
    color: #000; /* Couleur du texte */
    border: 1px solid #ccc; /* Bordure personnalisée */
    margin-top: 10px; /* Espacement au-dessus du bouton */
    padding: 10px 20px; /* Espacement interne du bouton */
    text-align: center; /* Centrage du texte */
    text-decoration: none; /* Suppression de soulignement du lien */
    display: inline-block; /* Pour avoir un bouton avec une taille personnalisée */
  }
  .custom-btn:hover {
    background-color: #e0e0e0; /* Changement de couleur sur hover */
    border-color: #bbb; /* Changement de couleur de bordure sur hover */
  }
</style>


								</form>
								
							</article>

						
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
