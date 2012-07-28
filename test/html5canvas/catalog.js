var catalog = {
    create: function(collections) {
		var parent = document.getElementsByTagName('body')[0];
		for (var i in collections) {
			var h1 = document.createElement('h1');
			parent.appendChild(h1);
			h1.innerHTML = i;

			var ul = document.createElement('ul');
			parent.appendChild(ul);

			var collection = collections[i];

			for (var j in collection.elements) {
				var element = collection.elements[j];
				var li = document.createElement('li');
				ul.appendChild(li);

				var canvas = document.createElement('canvas');
				canvas.width = 200;
				canvas.height = 200;
				li.appendChild(canvas);
				var context = canvas.getContext('2d');

				var scaleFactor = Math.min(canvas.width, canvas.height) / Math.max(element.bounds.width, element.bounds.height);
				collection.draw(context, j, 0, 0, element.bounds.width * scaleFactor, element.bounds.height * scaleFactor)
			}
		}
	}
}
