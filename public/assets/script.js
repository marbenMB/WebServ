const cookieBox = document.querySelector(".cookie__box");
acceptBtn = cookieBox.querySelector(".btn__group .accept__btn");
rejectBtn = cookieBox.querySelector(".btn__group .reject__btn");


rejectBtn.addEventListener('click' , ()=>{
    cookieBox.classList.add("hide")
})

acceptBtn.addEventListener('click' , async ()=>{
  await fetch('', {
  method: 'POST',
  headers: {
    'Content-Type': 'application/x-www-form-urlencoded'
  },
  body: 'login=username'
})

  location.reload()
})

