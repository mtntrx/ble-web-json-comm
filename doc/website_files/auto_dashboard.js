
        var Logger = {
            log: function() {
                var line = Array.prototype.slice.call(arguments).map(function(argument) {
                    return typeof argument === 'string' ? argument : JSON.stringify(argument);
                }).join(' ');

                document.querySelector('#log').textContent = '> ' + line + '\n' + document.querySelector('#log').textContent;
            },

            clearLog: function() {
                document.querySelector('#log').textContent = '';
            },
        };

	function parseAndDisplay( jsonString ) { 
	  	console.log("parseAndDisplay");
                var obj = JSON.parse(jsonString);
		for (var key in obj) {
	  		console.log(`key-value:  ${key}:${obj[key]}`);
			let masterDiv =  document.getElementById("masterDivId");
			let targetValDiv =  document.getElementById(key+"valDivId");
			if ( !targetValDiv ) {
				console.log("no target Div");
				let newKeyValDiv = document.createElement('div');
				let newKeyDiv = document.createElement('div');
				let newValDiv = document.createElement('div');

				newKeyValDiv.setAttribute('id',key + "keyValDivId");
				newKeyDiv.setAttribute('id',key + "keyDivId");
				newValDiv.setAttribute('id',key + "valDivId");

				masterDiv.appendChild(newKeyValDiv);
				newKeyValDiv.appendChild(newKeyDiv);
				newKeyValDiv.appendChild(newValDiv);

				newKeyDiv.classList.add("boxLabel");
				newValDiv.classList.add("boxValue");

				newKeyDiv.innerHTML = key;
				newValDiv.innerHTML = obj[key];

			} else {
				targetValDiv.innerHTML = obj[key];
			}
		}

	}

	
	setTimeout(function(){ parseAndDisplay('{"key1":"100", "key2":"200"}'); }, 1000);
	setTimeout(function(){ parseAndDisplay('{"key1":"300", "key3":"400"}'); }, 2000);
	setTimeout(function(){ parseAndDisplay('{"key1":"30", "key3":"20"}'); }, 3000);
	setTimeout(function(){ parseAndDisplay('{"key2":"75", "key3":"544"}'); }, 4000);
	setTimeout(function(){ parseAndDisplay('{"key2":"5gy", "key3":"544"}'); }, 4000);

