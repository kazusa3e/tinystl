devcontainer_name := tinystl

.PHONY: none
none:
	:

.PHONY: devcontainer.build
devcontainer.build:
	docker build -t $(devcontainer_name):latest .

.PHONY: devcontainer.run
devcontainer.run:
	docker run -d -v .:/app -w /app --name $(devcontainer_name) $(devcontainer_name):latest
