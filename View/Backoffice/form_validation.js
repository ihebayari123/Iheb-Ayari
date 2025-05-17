function validateOrderForm() {
  console.log("Validation en cours...");

  var customerName = document.getElementById("customer_name").value;
  var email = document.getElementById("customer_email").value;
  var productName = document.getElementById("product_name").value;
  var quantity = document.getElementById("quantity").value;
  var price = document.getElementById("price").value;

  if (customerName === "") {
    alert("Le nom du client est obligatoire !");
    return false;
  }

  var emailPattern = /^[a-zA-Z0-9._-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,6}$/;
  if (email === "") {
    alert("L'email est obligatoire !");
    return false;
  } else if (!emailPattern.test(email)) {
    alert("L'email n'est pas valide !");
    return false;
  }

  if (productName === "") {
    alert("Le nom du produit est obligatoire !");
    return false;
  }

  if (quantity === "" || isNaN(quantity) || quantity <= 0) {
    alert("Veuillez entrer une quantité valide !");
    return false;
  }

  if (price === "" || isNaN(price) || price <= 0) {
    alert("Veuillez entrer un prix valide !");
    return false;
  }

  return true;
}
