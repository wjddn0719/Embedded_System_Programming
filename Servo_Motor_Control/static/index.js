var setAngle = document.querySelector('#setAngle')

setAngle.addEventListener('click', function(){
    const angle = $('#angle').val();
    if(angle >= 0 && angle <= 180) {
        $.ajax({
            type: 'POST',
            url: '/api/angle',
            contentType: 'application/json',
            data: JSON.stringify({ angle: angle })
        }).done(function(result) {
            alert(result.message)
        }).fail(function(result) {
            console.log(result)
        })
    }
    else {
        alert("please enter an angle between 0 and 180!!!")
    }
});
