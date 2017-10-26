PROJECT := arangodb-php-driver
SCRIPTDIR := $(shell pwd)
ROOTDIR := $(shell cd $(SCRIPTDIR) && pwd)

GOBUILDDIR := $(SCRIPTDIR)/.gobuild
GOVERSION := 1.8.3-alpine
TMPDIR := $(GOBUILDDIR)

ORGPATH := github.com/sandrokeil
ORGDIR := $(GOBUILDDIR)/src/$(ORGPATH)
REPONAME := $(PROJECT)
REPODIR := $(ORGDIR)/$(REPONAME)
REPOPATH := $(ORGPATH)/$(REPONAME)

SOURCES := $(shell find . -name '*.go')

.PHONY: all build

all: build

build: $(GOBUILDDIR) $(SOURCES)
	GOPATH=$(GOBUILDDIR) go build -buildmode=c-archive -o arangodb.a main.go

$(GOBUILDDIR):
	@mkdir -p $(ORGDIR)
	GOPATH=$(GOBUILDDIR) go get github.com/arangodb/go-velocypack
