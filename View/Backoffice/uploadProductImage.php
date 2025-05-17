<?php
// Define the target directory
$targetDir = "../uploads/products/";
$targetFile = $targetDir . basename($_FILES["productImage"]["name"]);
$uploadOk = 1;
$imageFileType = strtolower(pathinfo($targetFile, PATHINFO_EXTENSION));

// Check if the file is an image
if (isset($_POST["submit"])) {
    $check = getimagesize($_FILES["productImage"]["tmp_name"]);
    if ($check !== false) {
        $uploadOk = 1;
    } else {
        echo "Le fichier n'est pas une image.";
        $uploadOk = 0;
    }
}

// Check if file already exists
if (file_exists($targetFile)) {
    echo "Désolé, le fichier existe déjà.";
    $uploadOk = 0;
}

// Limit file size (e.g., 5MB)
if ($_FILES["productImage"]["size"] > 5000000) {
    echo "Désolé, votre fichier est trop grand.";
    $uploadOk = 0;
}

// Allow only certain file formats (e.g., JPG, JPEG, PNG, GIF)
if ($imageFileType != "jpg" && $imageFileType != "jpeg" && $imageFileType != "png" && $imageFileType != "gif") {
    echo "Désolé, seuls les fichiers JPG, JPEG, PNG et GIF sont autorisés.";
    $uploadOk = 0;
}

// Check if $uploadOk is set to 0 by an error
if ($uploadOk == 0) {
    echo "Désolé, votre fichier n'a pas été téléchargé.";
} else {
    // If everything is ok, try to upload the file
    if (move_uploaded_file($_FILES["productImage"]["tmp_name"], $targetFile)) {
        echo "Le fichier " . htmlspecialchars(basename($_FILES["productImage"]["name"])) . " a été téléchargé avec succès.";
    } else {
        echo "Désolé, une erreur s'est produite lors du téléchargement de votre fichier.";
    }
}
?>
