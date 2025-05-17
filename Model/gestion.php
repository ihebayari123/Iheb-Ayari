<?php

class gestion {
    private ?int $id;
    private ?string $nom;
    private ?string $produit;
    private ?string $description;
    private ?DateTime $date;
    private string $statut; // kept non-nullable to avoid potential issues*
   

    // Constructor
    public function __construct(?int $id = null, ?string $nom = null, ?string $produit = null, 
                                ?string $description = null, ?DateTime $date = null, string $statut = 'Pending') {
        $this->id = $id;
        $this->nom = $nom;
        $this->produit = $produit;
        $this->description = $description;
        $this->date = $date;
        $this->statut = $statut;
   
    }

    // Getters and Setters

    public function getid(): ?int {
        return $this->id;
    }

    public function setid(?int $id): void {
        $this->id = $id;
    }

    public function getnom(): ?string {
        return $this->nom;
    }

    public function setnom(?string $nom): void {
        $this->nom = $nom;
    }

    public function getproduit(): ?string {
        return $this->produit;
    }

    public function setproduit(?string $produit): void {
        $this->produit = $produit;
    }

    public function getdescription(): ?string {
        return $this->description;
    }

    public function setdescription(?string $description): void {
        $this->description = $description;
    }

    public function getdate(): ?DateTime {
        return $this->date;
    }

    public function setdate(?DateTime $date): void {
        $this->date = $date;
    }

    public function getstatut(): string {
        return $this->statut;
    }

    public function setstatut(string $statut): void {
        $this->statut = $statut;
    }
    public function getemail() {
        return $this->email;
    }

    // Optional: Adding a toString method to display object details for easier debugging
    public function __toString(): string {
        return "Reclamation [ID: $this->id, Nom: $this->nom, Produit: $this->produit, Description: $this->description, Date: " 
            . ($this->date ? $this->date->format('Y-m-d') : 'N/A') . ", Statut: $this->statut]";
    }
    
}

?>
