<?php

class Product {
    private $id;
    private $name;
    private $description;
    private $price;
    private $availability;
    private $category;

    private const MAX_NAME_LENGTH = 255;
    private const MAX_DESCRIPTION_LENGTH = 1000;

    public function __construct(
        ?int $id = null,
        ?string $name = null,
        ?string $description = null,
        ?float $price = null,
        ?bool $availability = null,
        ?string $category = null
    ) {
        $this->setId($id);
        $this->setName($name);
        $this->setDescription($description);
        $this->setPrice($price);
        $this->setAvailability($availability);
        $this->setCategory($category);
    }

    public function getId(): ?int {
        return $this->id;
    }

    public function setId(?int $id): void {
        $this->id = $id;
    }

    public function getName(): ?string {
        return $this->name;
    }

    public function setName(?string $name): void {
        if (empty($name)) {
            throw new InvalidArgumentException("Name cannot be empty.");
        }
        if (strlen($name) > self::MAX_NAME_LENGTH) {
            throw new InvalidArgumentException("Name cannot exceed " . self::MAX_NAME_LENGTH . " characters.");
        }
        $this->name = htmlspecialchars(trim($name));
    }

    public function getDescription(): ?string {
        return $this->description;
    }

    public function setDescription(?string $description): void {
        if ($description !== null && strlen($description) > self::MAX_DESCRIPTION_LENGTH) {
            throw new InvalidArgumentException("Description cannot exceed " . self::MAX_DESCRIPTION_LENGTH . " characters.");
        }
        $this->description = htmlspecialchars(trim($description));
    }

    public function getPrice(): ?float {
        return $this->price;
    }

    public function setPrice(?float $price): void {
        if ($price !== null && $price <= 0) {
            throw new InvalidArgumentException("Price must be a positive number.");
        }
        $this->price = $price !== null ? round($price, 2) : null;
    }

    public function getAvailability(): ?bool {
        return $this->availability;
    }

    public function setAvailability(?bool $availability): void {
        if (!is_bool($availability) && $availability !== null) {
            throw new InvalidArgumentException("Availability must be a boolean value or null.");
        }
        $this->availability = $availability;
    }

    public function getCategory(): ?string {
        return $this->category;
    }

    public function setCategory(?string $category): void {
        if (empty($category)) {
            throw new InvalidArgumentException("Category cannot be empty.");
        }
        $this->category = htmlspecialchars(trim($category));
    }

    public function toArray(): array {
        return [
            'id' => $this->id,
            'name' => $this->name,
            'description' => $this->description,
            'price' => $this->price,
            'availability' => $this->availability,
            'category' => $this->category,
        ];
    }

    public static function fromArray(array $data): Product {
        return new Product(
            $data['id'] ?? null,
            $data['name'] ?? null,
            $data['description'] ?? null,
            $data['price'] ?? null,
            $data['availability'] ?? null,
            $data['category'] ?? null
        );
    }
}

?>
