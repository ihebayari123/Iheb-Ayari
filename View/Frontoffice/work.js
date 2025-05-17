


// Validation function
function validateForm() {
  const title = document.getElementById("title").value.trim();
  const content = document.getElementById("content").value.trim();
  const author = document.getElementById("author").value.trim();

  if (!title) {
    alert("Title is required.");
    return false;
  }
  if (!content) {
    alert("Content is required.");
    return false;
  }
  if (!author) {
    alert("Author is required.");
    return false;
  }

  return true; // Allow form submission
}

document.addEventListener('DOMContentLoaded', function() {
  const addButton = document.querySelector('.button-add');
  const formContainer = document.getElementById('discussion-form-container');

  addButton.addEventListener('click', function() {
      // Toggle the form visibility
      if (formContainer.style.display === "none" || formContainer.style.display === "") {
          formContainer.style.display = "flex";  // Show the form with flexbox
      } else {
          formContainer.style.display = "none";  // Hide the form
      }
  });
});










