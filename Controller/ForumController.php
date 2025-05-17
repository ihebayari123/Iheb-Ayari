<?php
include(__DIR__ . '/../config.php');
include(__DIR__ . '/../Model/Forum.php');

class PostController {
    public function listPosts($page = 1, $perPage = 15, $searchText = '') {
        $offset = ($page - 1) * $perPage;  // Calculate offset
    
        // Add WHERE clause to search by title if searchText is provided
        $sql = "SELECT * FROM post WHERE title LIKE :searchText LIMIT :limit OFFSET :offset";
        $db = config::getConnexion();
    
        try {
            $query = $db->prepare($sql);
            $query->bindValue(':searchText', '%' . $searchText . '%', PDO::PARAM_STR);  // Bind the search text with wildcard for LIKE
            $query->bindValue(':limit', $perPage, PDO::PARAM_INT);
            $query->bindValue(':offset', $offset, PDO::PARAM_INT);
            $query->execute();
            $list = $query->fetchAll();
            return $list;
        } catch(Exception $err) {
            echo $err->getMessage();
        }
    }
    

    public function getTotalPosts() {
        $sql = "SELECT COUNT(*) FROM post";
        $db = config::getConnexion();
    
        try {
            $query = $db->prepare($sql);
            $query->execute();
            $total = $query->fetchColumn();
            return $total;
        } catch(Exception $err) {
            echo $err->getMessage();
        }
    }
    
    

    public function addPost($post) {
        $sql = "INSERT INTO post (title, content, created_at, author) 
                VALUES (:title, :content, NOW(), :author)";
        $db = config::getConnexion();
    
        try {
            $query = $db->prepare($sql);
            $query->execute([
                'title' => $post->getTitle(),
                'content' => $post->getContent(),
                'author' => $post->getAuthor()
            ]);
        } catch (Exception $err) {
            echo $err->getMessage();
        }
    }

    public function updatePost($post) {
        $sql = "UPDATE post 
                SET title = :title, content = :content, author = :author, created_at = NOW() 
                WHERE id = :id";
        $db = config::getConnexion();
    
        try {
            $query = $db->prepare($sql);
            $query->execute([
                'id' => $post->getId(),
                'title' => $post->getTitle(),
                'content' => $post->getContent(),
                'author' => $post->getAuthor()
            ]);
        } catch (Exception $err) {
            die('Error: ' . $err->getMessage()); // Display the exact error
        }
    }
    
    

    public function deletePost($id) {
        $sql = "DELETE FROM post WHERE id = :id";
        $db = config::getConnexion();

        $query = $db->prepare($sql);
        $query->bindValue(':id', $id);

        try {
            $query->execute();
        } catch(Exception $e) {
            echo $e->getMessage();
        }
    }

    public function getPost($id) {
        $sql = "SELECT * FROM post WHERE id = :id";
        $db = config::getConnexion();
        $query = $db->prepare($sql);
        $query->bindValue(':id', $id);

        try {
            $query->execute();
            $post = $query->fetch();
            return $post;
        } catch(Exception $e) {
            echo $e->getMessage();
        }
    }

}

class CommentController {
    // Add Comment to a Post
    public function addComment($comment) {
        $sql = "INSERT INTO comment (post_id, content, created_at, author) 
                VALUES (:post_id, :content, NOW(), :author)";
        $db = config::getConnexion();
    
        try {
            $query = $db->prepare($sql);
            $query->execute([
                'post_id' => $comment->getPostId(),
                
                'content' => $comment->getcommentContent(),
                'author' => $comment->getcommentAuthor()
            ]);
        } catch (Exception $err) {
            echo $err->getMessage();
        }
    }

    // List Comments for a Post
    public function listComments($postId) {
        $sql = "SELECT * FROM comment WHERE post_id = :post_id ORDER BY created_at DESC";
        $db = config::getConnexion();
        $query = $db->prepare($sql);
        $query->bindValue(':post_id', $postId);

        try {
            $query->execute();
            $comments = $query->fetchAll();
            return $comments;
        } catch(Exception $e) {
            echo $e->getMessage();
        }
    }

    // Update a Comment
    public function updateComment($comment) {
        $sql = "UPDATE comment 
                SET comment_title = :comment_title, content = :content, author = :author, updated_at = NOW() 
                WHERE id = :id";
        $db = config::getConnexion();

        try {
            $query = $db->prepare($sql);
            $query->execute([
                'id' => $comment->getId(),
                'comment_title' => $comment->getCommentTitle(),
                'content' => $comment->getcommentContent(),
                'author' => $comment->getcommentAuthor()
            ]);
        } catch (Exception $err) {
            echo $err->getMessage();
        }
    }

    // Delete a Comment
    public function deleteComment($id) {
        $sql = "DELETE FROM comment WHERE id = :id";
        $db = config::getConnexion();

        $query = $db->prepare($sql);
        $query->bindValue(':id', $id);

        try {
            $query->execute();
        } catch(Exception $e) {
            echo $e->getMessage();
        }
    }

    
public function countComments($postId) {
    $query = "SELECT COUNT(*) FROM comment WHERE post_id = :post_id"; 
    $stmt = $this->db->prepare($query);
    $stmt->bindParam(':post_id', $postId, PDO::PARAM_INT);
    $stmt->execute();

    return $stmt->fetchColumn(); // This will return the count of comments
}


public function updateLikes($commentId, $isLike = true) {
    $column = $isLike ? 'likes' : 'dislikes';
    $query = "UPDATE comments SET $column = $column + 1 WHERE id = :id";
    $db = $this->getConnection();
    $stmt = $db->prepare($query);
    $stmt->bindParam(':id', $commentId, PDO::PARAM_INT);
    $stmt->execute();
}

public function getCommentLikesDislikes($commentId) {
    $query = "SELECT likes, dislikes FROM comments WHERE id = :id";
    $db = $this->getConnection();
    $stmt = $db->prepare($query);
    $stmt->bindParam(':id', $commentId, PDO::PARAM_INT);
    $stmt->execute();
    return $stmt->fetch(PDO::FETCH_ASSOC);
}


public function addReaction($postId, $userId, $reactionType) {
    $db = config::getConnexion();
    $sql = "INSERT INTO post_reactions (post_id, user_id, reaction_type) 
            VALUES (:post_id, :user_id, :reaction_type)
            ON DUPLICATE KEY UPDATE reaction_type = :reaction_type";

    try {
        $query = $db->prepare($sql);
        $query->execute([
            'post_id' => $postId,
            'user_id' => $userId,
            'reaction_type' => $reactionType
        ]);
    } catch (Exception $err) {
        echo $err->getMessage();
    }
}

public function countReactions($postId) {
    $db = config::getConnexion();
    $sql = "SELECT reaction_type, COUNT(*) as count FROM post_reactions 
            WHERE post_id = :post_id GROUP BY reaction_type";

    try {
        $query = $db->prepare($sql);
        $query->bindValue(':post_id', $postId, PDO::PARAM_INT);
        $query->execute();
        $reactions = $query->fetchAll(PDO::FETCH_ASSOC);

        $counts = ['like' => 0, 'dislike' => 0];
        foreach ($reactions as $reaction) {
            $counts[$reaction['reaction_type']] = $reaction['count'];
        }

        return $counts;
    } catch (Exception $err) {
        echo $err->getMessage();
    }
}

public function getUserReaction($postId, $userId) {
    $db = config::getConnexion();
    $sql = "SELECT reaction_type FROM post_reactions WHERE post_id = :post_id AND user_id = :user_id";
    try {
        $query = $db->prepare($sql);
        $query->execute([
            'post_id' => $postId,
            'user_id' => $userId
        ]);
        return $query->fetch(PDO::FETCH_ASSOC); // Returns the reaction type (like/dislike) or null if no reaction
    } catch (Exception $err) {
        echo $err->getMessage();
    }
}


}




?>
