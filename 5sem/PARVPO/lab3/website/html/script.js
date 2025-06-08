let selectedProductId = null;

// Функция для обработки выбора товара
document.querySelectorAll(".product").forEach(product => {
    product.addEventListener("click", () => {
        // Снимаем выделение с предыдущего товара
        document.querySelectorAll(".product").forEach(p => p.classList.remove("selected"));

        // Выделяем текущий товар
        product.classList.add("selected");

        // Запоминаем ID выбранного товара
        selectedProductId = product.dataset.id;

        // Активируем кнопку заказа
        document.getElementById("order-button").disabled = false;
    });
});

// Обработка нажатия кнопки заказа
document.getElementById("order-button").addEventListener("click", async () => {
    if (!selectedProductId) {
        alert("Выберите продукт!");
        return;
    }

    try {
        const response = await fetch('/order', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify({ product_id: selectedProductId }),
        });

        if (response.ok) {
            const data = await response.json();
            alert(`Заказ для позиции #${selectedProductId} успешно создан!`);
        } else {
            const error = await response.json();
            alert(`Ошибка: ${error.message}`);
        }
    } catch (err) {
        console.error('Request failed', err);
        alert('Failed to connect to the server.');
    }
});
