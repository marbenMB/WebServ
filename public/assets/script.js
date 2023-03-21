const cookieBox = document.querySelector(".cookie__box");
acceptBtn = cookieBox.querySelector(".btn__group .accept__btn");
rejectBtn = cookieBox.querySelector(".btn__group .reject__btn");


rejectBtn.addEventListener('click' , (e)=>{
    cookieBox.classList.add("hide")
})

acceptBtn.addEventListener('click' ,  async (e)=>{
  await fetch('http://127.0.0.1:8080/file.py', {
  method: 'POST',
  headers: {
    'Content-Type': 'application/x-www-form-urlencoded'
  },
  body: 'login=username'
})
  .then(response => response.json())
  .catch(error => console.error(error));

  location.reload()
})

