let inputs = document.querySelectorAll('input');

inputs.forEach(inp => {
    inp.addEventListener('change', updateValue)

    if(inp.type == 'file') {
    	// Firefox bug fix
    	inp.addEventListener( 'focus', function(){ inp.classList.add( 'has-focus' ); });
    	inp.addEventListener( 'blur', function(){ inp.classList.remove( 'has-focus' ); });
    }
});

function updateValue(e) {
    let input = e.target;
    let label = input.nextElementSibling;

    if(input.type != 'file') {
        if(!input.value || input.value == "")
            input.classList.remove("has-content");
        else
            input.classList.add("has-content");
    } else {
		let fileName = e.target.value.split( '\\' ).pop();

		if( fileName )
			label.querySelector( 'span' ).innerHTML = fileName;
		else
			label.querySelector( 'span' ).innerHTML = "Upload Solution";
    }
}

function attempt_login() {
    let login_form = document.querySelector('form[name="login"]');

    let username = document.querySelector('input[name="username"]').value;
    let password = document.querySelector('input[name="password"]').value;

    if(username && username != '' && password && password != '')
        login_form.submit();
}

function challenge_me() {
    let slots = document.querySelector('#slots');
    slots.style.marginLeft = "0px";

    let container = document.querySelector('#solution_upload');
    container.style.opacity = "1";
}

function cancel() {
    let slots = document.querySelector('#slots');
    slots.style.marginLeft = "-500px";

    let container = document.querySelector('#solution_upload');
    container.style.opacity = "0";

    let file = document.querySelector('input[type=file]');
    file.value = '';

    document.querySelector('.challenge_alert').style.opacity = "0";

    var changeEvent = document.createEvent("HTMLEvents");
    changeEvent.initEvent("change", false, true);
    file.dispatchEvent(changeEvent);
}

function submit_solution() {
    document.querySelector('form[name="challenge"]').submit()
}
