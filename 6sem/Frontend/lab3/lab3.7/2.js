const circle = document.querySelector('.circle');

document.addEventListener('mousemove', (event) => {
    circle.style.left = `${event.pageX - 30}px`; 
    circle.style.top = `${event.pageY - 30}px`;  
});