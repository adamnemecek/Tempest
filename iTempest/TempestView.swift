//
//  TempestView.swift
//  iTempest
//
//  Created by Randy Rasmussen on 11/15/17.
//  Copyright © 2017 Randy Rasmussen. All rights reserved.
//

import Foundation
import UIKit
import MetalKit

class TempestView : MTKView {
   private var commandQueue: MTLCommandQueue?
   private var depthStencilState: MTLDepthStencilState?
   private var tempest : cTempest = 0
   private var tempestRenderer : TempestViewRenderer?
   private var spinner : SpinnerView?
   private var spinnerRenderer : SpinnerRenderer?
   private var spinnerHub : SpinnerHub?
   private var player1 : PlayerButtonView?
   private var player2 : PlayerButtonView?
   private var fire : ButtonView?
   private var zap : ButtonView?

   init(tempest : cTempest) {
      // call the super
      super.init(frame: CGRect(), device: MTLCreateSystemDefaultDevice())
      
      // save parameters
      self.tempest = tempest
      
      // View
      clearColor = MTLClearColorMake(0.0, 0.0, 0.0, 1)
      colorPixelFormat = .bgra8Unorm
      depthStencilPixelFormat = .depth32Float
      
      // Command queue
      commandQueue = device!.makeCommandQueue()      
      
      // Depth stencil
      let depthSencilDescriptor = MTLDepthStencilDescriptor()
      depthSencilDescriptor.depthCompareFunction = .less
      depthSencilDescriptor.isDepthWriteEnabled = true
      depthStencilState = device!.makeDepthStencilState(descriptor: depthSencilDescriptor)
      
      // now that we're initialized we can create the renderers
      tempestRenderer = TempestViewRenderer(view: self, tempest: tempest)
      spinnerRenderer = SpinnerRenderer(view:self)
      spinnerHub = SpinnerHub(view:self)
      player1 = PlayerButtonView(view:self, tempest:tempest, whichButton:PLAYER1)
      player2 = PlayerButtonView(view:self, tempest:tempest, whichButton:PLAYER2)
      fire = ButtonView(view:self, shaderName:"playerButtonShader", tempest:tempest, whichButton:FIRE)
      zap = ButtonView(view:self, shaderName:"playerButtonShader", tempest:tempest, whichButton:ZAP)

      // add our child views... our views are actually just transparent place holders for
      // receiving user inputs
      spinner = SpinnerView(tempest: tempest, callback:onSpinnerMoved)
      self.addSubview(spinner!)
      self.addSubview(player1!)
      self.addSubview(player2!)
      self.addSubview(fire!)
      self.addSubview(zap!)
   }

   required init(coder: NSCoder) {
      super.init(coder: coder)
   }
   
   /**
    * Called on every frame to perform Metal rendering
    */
   override func draw(_ dirtyRect: CGRect) {
      // create our parallel render encoder
      let commandBuffer = commandQueue!.makeCommandBuffer()
      let renderPassDescriptor = currentRenderPassDescriptor!
      let parallelRenderEncoder = commandBuffer.makeParallelRenderCommandEncoder(descriptor: renderPassDescriptor)

      // render stuff
      render(parallelRenderEncoder:parallelRenderEncoder,renderer:tempestRenderer!)
      render(parallelRenderEncoder:parallelRenderEncoder,renderer:spinnerRenderer!)
      render(parallelRenderEncoder:parallelRenderEncoder,renderer:spinnerHub!)
      render(parallelRenderEncoder:parallelRenderEncoder,renderer:player1!)
      render(parallelRenderEncoder:parallelRenderEncoder,renderer:player2!)
      render(parallelRenderEncoder:parallelRenderEncoder,renderer:fire!)
      render(parallelRenderEncoder:parallelRenderEncoder,renderer:zap!)

      // end encoding on our parallel renderer
      parallelRenderEncoder.endEncoding()
      
      let drawable = currentDrawable!
      commandBuffer.present(drawable)
      commandBuffer.commit()
   }

   private func render(parallelRenderEncoder:MTLParallelRenderCommandEncoder, renderer:AbstractMetalRenderer) {
      // create our render encoder for tempest and render it
      let renderEncoder = parallelRenderEncoder.makeRenderCommandEncoder()
      renderEncoder.setFrontFacing(.counterClockwise)
      renderEncoder.setDepthStencilState(depthStencilState)
      renderer.render(renderEncoder: renderEncoder)
      renderEncoder.endEncoding()
   }
   
   override func layoutSubviews() {
      // call the super
      super.layoutSubviews()
      
      player1!.frame = CGRect(x:0.0, y:0.0, width:50, height:50)
      player2!.frame = CGRect(x:self.frame.width-50, y:0.0, width:50, height:50)

      // set the frame of the Tempest graphic
      var width = self.frame.width
      if (width > self.frame.height) {
         width = self.frame.height
      }
      tempestRenderer!.frame = CGRect(x:0.0, y:0.0, width:width, height:width)

      // figuring out if we are portrait or landscape
      if (self.frame.width > self.frame.height) {
         // spinner
         spinnerRenderer!.frame = CGRect(
            x: tempestRenderer!.frame.maxX,
            y: self.frame.height - 80,
            width: self.frame.width - tempestRenderer!.frame.maxX,
            height: 80);
      } else {
         // spinner
         spinnerRenderer!.frame = CGRect(
            x: 0,
            y: self.frame.height - 80,
            width: self.frame.width,
            height: 80);
      }
      
      // for the moment just always put fire and zap over the spinner
      fire!.frame = CGRect(
         x: spinnerRenderer!.frame.minX,
         y: spinnerRenderer!.frame.minY-80,
         width:80,
         height:80
      )
      zap!.frame = CGRect(
         x: spinnerRenderer!.frame.maxX-80,
         y: spinnerRenderer!.frame.minY-80,
         width:80,
         height:80
      )

      // and the actual spinner view is the same size... this is just an invisible control
      // that handles the touch input
      spinner!.frame = spinnerRenderer!.frame
      spinnerHub!.frame = spinnerRenderer!.frame
   }
   
   private func onSpinnerMoved(ticks : Int) {
      // convert our encoder tick count to a rotation angle using our magic constant
      let rotation = 1.0 * Float(ticks)
      
      // rotate our graphics
      spinnerRenderer!.rotate(rotationAngle:rotation)
      spinnerHub!.rotate(rotationAngle:rotation)
   }
}
