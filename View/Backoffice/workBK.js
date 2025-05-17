document.addEventListener('DOMContentLoaded', function () {
    const searchInput = document.querySelector('.inputsearch');
    const postListContainer = document.getElementById('forum-list');

    searchInput.addEventListener('input', function () {
        const searchText = searchInput.value.trim();

        // AJAX request to fetch posts (filtered or all)
        const xhr = new XMLHttpRequest();
        xhr.open('GET', `forumList.php?text=${encodeURIComponent(searchText)}`, true);

        xhr.onload = function () {
            if (xhr.status === 200) {
                // Update forum list dynamically
                postListContainer.innerHTML = xhr.responseText;

                // Reapply CSS and other behaviors if necessary
                applyDynamicStyles();
            }
        };

        xhr.send();
    });
});

// Function to reapply dynamic styles (if required)
function applyDynamicStyles() {
    // Reinitialize any JavaScript-driven behaviors or styles
    // For example, animations, click handlers, etc.
}
